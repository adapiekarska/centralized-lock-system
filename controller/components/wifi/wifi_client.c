#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "tcpip_adapter.h"

// socket-related includes
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

// project-related includes
#include "types.h"
#include "wifi_client.h"
#include "wifi_status.h"

// TODO: define address and port

#define SERVER_IP_ADDR "192.168.101.59"
#define SERVER_PORT CONFIG_SERVER_PORT

static char         pending_data[128];
static unsigned int pending_data_size; 

static const char *LOG_TAG = "wifi_client";

void wifi_client_start()
{
    // Task wifi_client should wait for connection
    wifi_status_wait_bits(WIFI_CONNECTED_BIT, DONT_CLEAR);

    char rx_buffer[128];
    char addr_str[128];

    while (TRUE)
    {
        // Create a socket
        int sock =  socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
        if (sock < 0)
        {
            ESP_LOGE(LOG_TAG, "Unable to create socket: errno %d", errno);
            break;
        }

        // Specify server data
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(SERVER_PORT);
        // Inet_addr converts the Internet host address cp from IPv4 numbers-and-dots notation into binary data in network byte order
        server_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);

        // Convert an Internet address into a string
        inet_ntoa_r(server_addr.sin_addr, addr_str, sizeof(addr_str) - 1);

        ESP_LOGI(LOG_TAG, "Socket created, connecting to %s:%d", SERVER_IP_ADDR, SERVER_PORT);

        // Connect socket to the server
        int err = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (err != 0)
        {
            ESP_LOGE(LOG_TAG, "Socket unable to connect: errno %d", errno);
            break;
        }

        ESP_LOGI(LOG_TAG, "Successfully connected");

        // Communication loop
        while (TRUE)
        {
            // Notify capability to transfer data
            wifi_status_set_bits(WIFI_CLIENT_READY_BIT);
            // wait for pending data
            wifi_status_wait_bits(WIFI_CLIENT_DATA_PENDING_BIT, CLEAR);
            
            // Send data
            int err = send(sock, pending_data, pending_data_size, 0);

            // Error occurred during sending
            if (err < 0)
            {
                ESP_LOGE(LOG_TAG, "Error occurred during sending: errno %d", errno);
                // Notify transmission fail
                wifi_status_set_bits(WIFI_CLIENT_TRANSMISSION_FAIL_BIT);
                break;
            }

             // Notify transmission success
            wifi_status_set_bits(WIFI_CLIENT_TRANSMISSION_SUCCESS_BIT);

            // Receive data
            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);

            // Error occurred during receiving
            if (len < 0)
            {
                ESP_LOGE(LOG_TAG, "Error occurred during receiving: errno %d", errno);
                break;
            }

            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }

        if (sock != -1)
        {
            ESP_LOGE(LOG_TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
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
