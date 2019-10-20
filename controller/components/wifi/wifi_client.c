#include "wifi_client.h"

#include <sys/param.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_err.h"

// project-related includes
#include "types.h"
#include "wifi_socket/wifi_socket.h"
#include "wifi_tls/wifi_tls.h"
#include "wifi_conifg.h"
#include "controller_status.h"


static char         pending_data[128];
static size_t       pending_data_size;

void wifi_client_start()
{
    // Task wifi_client should wait for connection to AP
    controller_status_wait_bits(WIFI_CONNECTED_BIT, DONT_CLEAR);

    // Receive buffer
    char rx_buffer[128];
    size_t rx_buffer_size = 128;

    esp_err_t err;

    while (TRUE)
    {
        err = TLS_ENABLED ?
            wifi_tls_connect()
            : wifi_socket_connect();

        if (err == ESP_FAIL)
        {
            break;
        }

        // Communication loop
        while (TRUE)
        {
            ESP_LOGE("wifi_client", "Ready to transmit");

            controller_status_set_bits(WIFI_CLIENT_READY_BIT);
            // Wait for pending data
            controller_status_wait_bits(WIFI_CLIENT_DATA_PENDING_BIT, CLEAR);

            // Send data
            err = TLS_ENABLED ?
                    wifi_tls_transfer_data(pending_data, pending_data_size)
                    : wifi_socket_transfer_data(pending_data, pending_data_size);

            if (err == ESP_FAIL)
            {
                // Notify transmission fail
                controller_status_set_bits(WIFI_CLIENT_TRANSMISSION_FAIL_BIT);
                break;
            }

             // Notify transmission success
            controller_status_set_bits(WIFI_CLIENT_TRANSMISSION_SUCCESS_BIT);

            // Receive data
            err = TLS_ENABLED ?
                    wifi_tls_receive_data(rx_buffer, rx_buffer_size)
                    : wifi_socket_receive_data(rx_buffer, rx_buffer_size);

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
    controller_status_wait_bits(WIFI_CLIENT_READY_BIT, CLEAR);

    memcpy(pending_data, data, data_size);
    pending_data_size = data_size;

    // Notify wifi_client that there's new data to transfer
    controller_status_set_bits(WIFI_CLIENT_DATA_PENDING_BIT);

    // Wait for transmission to finish
    controller_status_wait_bits(
        WIFI_CLIENT_TRANSMISSION_SUCCESS_BIT | WIFI_CLIENT_TRANSMISSION_FAIL_BIT,
        DONT_CLEAR
        );

    if (controller_status_get_bit(WIFI_CLIENT_TRANSMISSION_SUCCESS_BIT))
    {
        // Transmission was succesfull
        // Clear WIFI_CLIENT_TRANSMISSION_SUCCESS_BIT and return success code
        controller_status_clear_bits(WIFI_CLIENT_TRANSMISSION_SUCCESS_BIT);
        return ESP_OK;
    }
    else
    {
        // Transmission failed
        // Clear WIFI_CLIENT_TRANSMISSION_FAIL_BIT and return fail code
        controller_status_clear_bits(WIFI_CLIENT_TRANSMISSION_FAIL_BIT);
        return ESP_FAIL;
    }
}
