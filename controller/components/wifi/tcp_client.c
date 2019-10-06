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
#include "tcp_client.h"
#include "wifi_status.h"

// TODO: define address and port

#define SERVER_IP_ADDR "192.168.101.59"
#define SERVER_PORT CONFIG_SERVER_PORT

static const char *LOG_TAG = "tcp_client";
static const char *payload = "Message from ESP32 ";

void start_tcp_client()
{
    // Task TCP Client should wait for connection
    wifi_status_wait_bits(WIFI_CONNECTED_BIT);

    char rx_buffer[128];
    char addr_str[128];

    while (1)
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
        while (1)
        {
            // Send data
            int err = send(sock, payload, strlen(payload), 0);

            // Error occurred during sending
            if (err < 0)
            {
                ESP_LOGE(LOG_TAG, "Error occurred during sending: errno %d", errno);
                break;
            }

            // Receive data
            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);

            // Error occurred during receiving
            if (len < 0)
            {
                ESP_LOGE(LOG_TAG, "Error occurred during receiving: errno %d", errno);
                break;
            }

            // Data received
            else
            {
                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                ESP_LOGI(LOG_TAG, "Received %d bytes from %s:", len, addr_str);
                ESP_LOGI(LOG_TAG, "%s", rx_buffer);
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
