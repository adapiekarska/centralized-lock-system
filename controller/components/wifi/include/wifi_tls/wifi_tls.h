#ifndef WIFI_TLS_H
#define WIFI_TLS_H

#include "esp_err.h"

/**
 * @brief Connect via TLS
 *
 * @return esp_err_t status
 */
esp_err_t wifi_tls_connect();

/**
 * @brief Transfer data via TLS
 *
 * @param data pointer to data
 * @param size size
 * @return esp_err_t status
 */
esp_err_t wifi_tls_transfer_data(
    char *data,
    size_t size
    );

/**
 * @brief Receive data via TLS
 *
 * @param rx_buffer output buffer
 * @param size size
 * @return esp_err_t status
 */
esp_err_t wifi_tls_receive_data(
    char *rx_buffer,
    size_t size
    );

/**
 * @brief Shutdown the TLS
 *
 */
void wifi_tls_shutdown();

#endif // WIFI_TLS_H