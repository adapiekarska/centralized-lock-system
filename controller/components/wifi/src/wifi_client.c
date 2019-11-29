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

    esp_err_t err;

    while (TRUE)
    {
        err = CONFIG_TLS_ENABLED ?
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
            
            // Wait for pending operation
            controller_status_wait_bits(
                WIFI_CLIENT_SEND_PENDING_BIT | WIFI_CLIENT_RECEIVE_PENDING_BIT,
                DONT_CLEAR
                );

            if (controller_status_get_bit(WIFI_CLIENT_SEND_PENDING_BIT))
            {
                // Requested operation is SEND
                // Clear WIFI_CLIENT_SEND_PENDING_BIT
                controller_status_clear_bits(WIFI_CLIENT_SEND_PENDING_BIT);

                // Send data
                err = CONFIG_TLS_ENABLED ?
                        wifi_tls_transfer_data(pending_data, pending_data_size)
                        : wifi_socket_transfer_data(pending_data, pending_data_size);
                if (err == ESP_FAIL)
                {
                    // Notify transmission fail
                    controller_status_set_bits(WIFI_CLIENT_TRANSMISSION_FAIL_BIT);
                    continue; // TODO: What should be done when data transmission fails?
                }
            }
            else
            {
                // Requested operation is RECEIVE
                // Clear WIFI_CLIENT_RECEIVE_PENDING_BIT
                controller_status_clear_bits(WIFI_CLIENT_RECEIVE_PENDING_BIT);

                // Receive data
                err = CONFIG_TLS_ENABLED ?
                        wifi_tls_receive_data(pending_data, pending_data_size)
                        : wifi_socket_receive_data(pending_data, pending_data_size);

                if (err == ESP_FAIL)
                {
                    // Notify transmission fail
                    controller_status_set_bits(WIFI_CLIENT_TRANSMISSION_FAIL_BIT);
                    continue; // TODO: What should be done when data transmission fails?
                }
            }
      
            // Notify transmission success
            controller_status_set_bits(WIFI_CLIENT_TRANSMISSION_SUCCESS_BIT);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }

        CONFIG_TLS_ENABLED ? wifi_tls_shutdown() : wifi_socket_shutdown();
    }

    // Remove task from kernel's management
    vTaskDelete(NULL);
}

esp_err_t wifi_client_send_data(
    void    *data,
    int     data_size
    )
{
    esp_err_t status;
    // Wait for the wifi_client task to notify capability to send data
    status = controller_status_wait_bits_timeout(
        WIFI_CLIENT_READY_BIT, 
        CLEAR, 
        WIFI_CLIENT_READY_TIMEOUT
        );
    if (status == ESP_ERR_TIMEOUT)
    {
        return ESP_ERR_TIMEOUT;
    }

    memcpy(pending_data, data, data_size);
    pending_data_size = data_size;

    // Notify wifi_client that there's new data to transfer
    controller_status_set_bits(WIFI_CLIENT_SEND_PENDING_BIT);

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

esp_err_t wifi_client_receive_data(
    void    *buffer,
    int     buffer_size
    )
{
    esp_err_t status;
    // Wait for the wifi_client task to notify capability to send data
    status = controller_status_wait_bits_timeout(
        WIFI_CLIENT_READY_BIT, 
        CLEAR, 
        WIFI_CLIENT_READY_TIMEOUT
        );
    if (status == ESP_ERR_TIMEOUT)
    {
        return ESP_ERR_TIMEOUT;
    }

    // Notify wifi_client that there's new data to transfer
    controller_status_set_bits(WIFI_CLIENT_RECEIVE_PENDING_BIT);

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
        memcpy(buffer, pending_data, buffer_size);
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
