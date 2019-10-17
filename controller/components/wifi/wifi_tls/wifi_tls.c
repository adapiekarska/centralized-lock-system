#include "wifi_tls/wifi_tls.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_err.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "esp_tls.h"

#include "../include/wifi_conifg.h"

static const char *LOG_TAG = "wifi_tls";

// Embedded binary file - CA certificate
extern const uint8_t ca_cer_start[] asm("_binary_ca_cer_start");
extern const uint8_t ca_cer_end[]   asm("_binary_ca_cer_end");

static esp_tls_t *tls_conn;
static esp_tls_cfg_t tls_cfg;

esp_err_t wifi_tls_connect()
{
    char addr_str[128];

    server_addr.sin_port = htons(SERVER_PORT);
    // Inet_addr converts the Internet host address cp from IPv4 numbers-and-dots notation into binary data in network byte order
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);

    // Convert an Internet address into a string
    inet_ntoa_r(server_addr.sin_addr, addr_str, sizeof(addr_str) - 1);

    tls_cfg =
    {
        .cacert_buf  = ca_cer_start,
        .cacert_bytes = ca_cer_end - ca_cer_start,
    };

    tls_conn = esp_tls_conn_http_new(addr_str, &tls_cfg);

    if (tls_conn != NULL)
    {
        ESP_LOGI(LOG_TAG, "Connection established...");
        return ESP_OK;
    }
    else
    {
        ESP_LOGE(LOG_TAG, "Connection failed...");
        return ESP_FAIL;
    }
}

esp_err_t wifi_tls_transfer_data(char *data, size_t size)
{
    size_t bytes_written = 0;
    do
    {
        ret = esp_tls_conn_write(
            tls_conn, data + bytes_written,
            strlen(data) - bytes_written
            );

        if (ret >= 0)
        {
            ESP_LOGI(LOG_TAG, "%d bytes written", ret);
            bytes_written += ret;
        }
        else if (ret != MBEDTLS_ERR_SSL_WANT_READ  && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
            ESP_LOGE(LOG_TAG, "esp_tls_conn_write  returned 0x%x", ret);
            return ESP_FAIL;
        }
    }
    while (bytes_written < size);

    return ESP_OK;
}

esp_err_t wifi_tls_receive_data(char *rx_buffer)
{
    esp_err_t ret;
    do
        {
            len = sizeof(buf) - 1;
            bzero(buf, sizeof(buf));
            ret = esp_tls_conn_read(tls, (char *)buf, len);

            if(ret == MBEDTLS_ERR_SSL_WANT_WRITE  || ret == MBEDTLS_ERR_SSL_WANT_READ)
                continue;

            if(ret < 0)
           {
                ESP_LOGE(LOG_TAG, "esp_tls_conn_read  returned -0x%x", -ret);
                return ESP_FAIL;
            }

            if(ret == 0)
            {
                ESP_LOGI(LOG_TAG, "connection closed");
                return ESP_FAIL;
            }

            len = ret;
            ESP_LOGD(TAG, "%d bytes read", len);
            /* Print response directly to stdout as it is read */
            for(int i = 0; i < len; i++) {
                putchar(buf[i]);
            }
        } while(1);

    return ESP_OK;
}

void wifi_tls_shutdown()
{
    esp_tls_conn_delete(tls_conn);
}