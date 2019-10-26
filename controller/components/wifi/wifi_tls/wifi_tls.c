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
#include "types.h"

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


static void zeromem(void* ptr, size_t len){
    for(int i = 0; i < len; i++){
        ((char*)ptr)[i] = 0;
    }
}

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

    // Set common name
    tls_cfg.common_name = "AUTH-SERV";

    // Set non-blocking mode
    // tls_cfg.non_block = TRUE;

    tls_conn = esp_tls_init();

    // Non-Blocking TLS/SSL connection
    // esp_tls_conn_new_async(SERVER_HOSTNAME, sizeof(SERVER_HOSTNAME), SERVER_PORT, &tls_cfg, tls_conn);

    esp_tls_conn_new_sync(SERVER_HOSTNAME, sizeof(SERVER_HOSTNAME), SERVER_PORT, &tls_cfg, tls_conn);


    // Wait for
    while (tls_conn->conn_state != ESP_TLS_DONE && tls_conn->conn_state != ESP_TLS_FAIL)
    {
        ESP_LOGE(LOG_TAG, "Waiting for connection, current state: %d", tls_conn->conn_state);
        // Delay for 0.5 s - 50 ticks - each tick is 10 ms
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    if (tls_conn->conn_state == ESP_TLS_DONE)
    {
        return ESP_OK;
    }
    else
    {
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
            tls_conn,
            data + bytes_written,
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
        zeromem(rx_buffer, size);
        ret = esp_tls_conn_read(tls_conn, rx_buffer, size);

        if(ret == MBEDTLS_ERR_SSL_WANT_WRITE || ret == MBEDTLS_ERR_SSL_WANT_READ){
            // small delay to reset watchdog
            vTaskDelay(20 / portTICK_PERIOD_MS);
            continue;
        }

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

        ESP_LOGD(LOG_TAG, "%d bytes received", ret);
        break;
    } while(1);

    return ESP_OK;
}

void wifi_tls_shutdown()
{
    esp_tls_conn_delete(tls_conn);
}