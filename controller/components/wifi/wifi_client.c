#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"
#include "tcpip_adapter.h"

// project-related includes
#include "types.h"
#include "wifi_client.h"
#include "wifi_status.h"
#include "wifi_socket.h"
#include "wifi_tls.h"
#include "wifi_config.h"


static char         pending_data[128];
static unsigned int pending_data_size;

static const char *LOG_TAG = "wifi_client";

void wifi_client_start()
{
    // Task wifi_client should wait for connection to AP
    wifi_status_wait_bits(WIFI_CONNECTED_BIT, DONT_CLEAR);

    // Receive buffer
    char rx_buffer[128];

    esp_err_t err;

    while (TRUE)
    {
        err = TLS_ENABLED ? wifi_tls_connect() : wifi_socket_connect();

        if (err == ESP_FAIL)
        {
            break;
        }

        // Communication loop
        while (TRUE)
        {
            // Notify capability to transfer data
            wifi_status_set_bits(WIFI_CLIENT_READY_BIT);

            // Wait for pending data
            wifi_status_wait_bits(WIFI_CLIENT_DATA_PENDING_BIT, CLEAR);

            // Send data
            err = TLS_ENABLED ? wifi_tls_transfer_data() : wifi_socket_connect();

            if (err == ESP_FAIL)
            {
                wifi_status_set_bits(WIFI_CLIENT_TRANSMISSION_FAIL_BIT);
                break;
            }

             // Notify transmission success
            wifi_status_set_bits(WIFI_CLIENT_TRANSMISSION_SUCCESS_BIT);

            // Receive data
            err = TLS_ENABLED ? wifi_tls_receive_data(rx_buffer) : wifi_socket_receive_data(rx_buffer);

            if (err == ESP_FAIL)
            {
                break;
            }

            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }

        TLS_ENABLED ? wifi_tls_shutdown() : wifi_socket_shutdown();
    }

    // Remove task from kernel's management
    vTaskDelete(NULL);
}

esp_err_t wifi_client_transfer_data(
    void    *data,
    int     data_size
    )
{
    // Wait for the wifi_client task to notify capability to send data
    wifi_status_wait_bits(WIFI_CLIENT_READY_BIT, CLEAR);

    memcpy(pending_data, data, data_size);
    pending_data_size = data_size;

    // Notify wifi_client that there's new data to transfer
    wifi_status_set_bits(WIFI_CLIENT_DATA_PENDING_BIT);

    // Wait for transmission to finish
    wifi_status_wait_bits(
        WIFI_CLIENT_TRANSMISSION_SUCCESS_BIT | WIFI_CLIENT_TRANSMISSION_FAIL_BIT,
        DONT_CLEAR
        );

    if (wifi_status_get_bit(WIFI_CLIENT_TRANSMISSION_SUCCESS_BIT))
    {
        // Transmission was succesfull
        // Clear WIFI_CLIENT_TRANSMISSION_SUCCESS_BIT and return success code
        wifi_status_clear_bits(WIFI_CLIENT_TRANSMISSION_SUCCESS_BIT);
        return ESP_OK;
    }
    else
    {
        // Transmission failed
        // Clear WIFI_CLIENT_TRANSMISSION_FAIL_BIT and return fail code
        wifi_status_clear_bits(WIFI_CLIENT_TRANSMISSION_FAIL_BIT);
        return ESP_FAIL;
    }
}
