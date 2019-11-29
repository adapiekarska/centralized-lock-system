#include "wifi_socket/wifi_socket.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"
#include "tcpip_adapter.h"

// socket-related includes
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "../include/wifi_conifg.h"

static const char *LOG_TAG = "wifi_sock";

static int sock;

esp_err_t wifi_socket_connect()
{
    char addr_str[128];

    // Create a socket
    sock =  socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0)
    {
        ESP_LOGE(LOG_TAG, "Unable to create socket: errno %d", errno);
        return ESP_FAIL;
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
        return ESP_FAIL;
    }

    ESP_LOGI(LOG_TAG, "Successfully connected");
    return ESP_OK;
}

esp_err_t wifi_socket_transfer_data(
    char *data,
    size_t size
    )
{
    // Send data
    int err = send(sock, data, size, 0);

    // Error occurred during sending
    if (err < 0)
    {
        ESP_LOGE(LOG_TAG, "Error occurred during sending: errno %d", errno);
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t wifi_socket_receive_data(
    char *rx_buffer,
    size_t size
    )
{
    // Receive data
    int len = recv(sock, rx_buffer, size - 1, 0);

    // Error occurred during receiving
    if (len < 0)
    {
        ESP_LOGE(LOG_TAG, "Error occurred during receiving: errno %d", errno);
        return ESP_FAIL;
    }
    return ESP_OK;
}

void wifi_socket_shutdown()
{
    if (sock != -1)
    {
        ESP_LOGE(LOG_TAG, "Shutting down socket and restarting...");
        shutdown(sock, 0);
        close(sock);
    }
}