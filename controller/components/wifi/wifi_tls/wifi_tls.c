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

#include "mbedtls/net.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/debug.h"

#include "esp_tls.h"

#include "../include/wifi_conifg.h"

static const char *LOG_TAG = "wifi_tls";

// Embedded binary file - CA certificate
extern const uint8_t ca_cer_start[] asm("_binary_ca_cer_start");
extern const uint8_t ca_cer_end[]   asm("_binary_ca_cer_end");

// Embedded binary file - client certificate
extern const uint8_t client_cer_start[] asm("_binary_client_cer_start");
extern const uint8_t client_cer_end[]   asm("_binary_client_cer_end");

// Embedded binary file - client private key
extern const uint8_t client_pkey_start[] asm("_binary_client_key_start");
extern const uint8_t client_pkey_end[]   asm("_binary_client_key_end");

// TLS connection and config
static esp_tls_t *tls_conn;
static esp_tls_cfg_t tls_cfg;


esp_err_t wifi_tls_connect()
{
    // Set up root CA certificate
    tls_cfg.cacert_buf = ca_cer_start;
    tls_cfg.cacert_bytes = ca_cer_end - ca_cer_start;

    // Set up client certificate
    tls_cfg.clientcert_buf = client_cer_start;
    tls_cfg.clientcert_bytes = client_cer_end - client_cer_start;

    // Set up client key
    tls_cfg.clientkey_buf = client_pkey_start;
    tls_cfg.clientkey_bytes = client_pkey_end - client_pkey_start;

    // Blocking TLS/SSL connection
    int status = esp_tls_conn_new_sync(SERVER_HOSTNAME, sizeof(SERVER_HOSTNAME), SERVER_PORT, &tls_cfg, tls_conn);

    if (status == 1)
    {
        ESP_LOGI(LOG_TAG, "Connection established...");
        return ESP_OK;
    }
    else if (status == -1)
    {
        ESP_LOGE(LOG_TAG, "Connection failed...");
        return ESP_FAIL;
    }
    else
    {
        ESP_LOGI(LOG_TAG, "Connection in progress, but shouldn't be...");
        return ESP_FAIL;
    }

}

esp_err_t wifi_tls_transfer_data(
    char *data,
    size_t size
    )
{
    esp_err_t ret;
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
            ESP_LOGE(LOG_TAG, "esp_tls_conn_write returned 0x%x", ret);
            return ESP_FAIL;
        }
    }
    while (bytes_written < size);

    return ESP_OK;
}

esp_err_t wifi_tls_receive_data(
    char *rx_buffer,
    size_t size
    )
{
    esp_err_t ret;
    do
        {
            size_t len = size - 1;
            bzero(rx_buffer, size);
            ret = esp_tls_conn_read(tls_conn, rx_buffer, len);

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
            ESP_LOGD(LOG_TAG, "%d bytes read", len);
            /* Print response directly to stdout as it is read */
            for(int i = 0; i < len; i++) {
                putchar(rx_buffer[i]);
            }
        } while(1);

    return ESP_OK;
}

void wifi_tls_shutdown()
{
    esp_tls_conn_delete(tls_conn);
}