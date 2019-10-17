#ifndef WIFI_SOCKET_H
#include WIFI_SOCKET_H

#include "esp_err.h"

/**
 * @brief Connect to socket
 *
 * @return esp_err_t status
 */
esp_err_t wifi_socket_connect();

/**
 * @brief Transfer data via socket
 *
 * @param data pointer to data
 * @param size size
 * @return esp_err_t status
 */
esp_err_t wifi_socket_transfer_data(char *data, size_t size);

/**
 * @brief Receive data via socket
 *
 * @param rx_buffer output buffer
 * @return esp_err_t status
 */
esp_err_t wifi_socket_receive_data(char *rx_buffer);

/**
 * @brief Shutdown the socket
 *
 */
void wifi_socket_shutdown();

#endif // WIFI_SOCKET_H