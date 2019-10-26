#ifndef WIFI_CLIENT_H
#define WIFI_CLIENT_H

#include "esp_err.h"

/**
 * @brief Start the process of data exchange with server
 *
 */
void wifi_client_start();

/**
 * @brief Send given data to attestation server
 *
 * @param data      pointer to buffer containing data to be sent
 * @param data_size data buffer size
 *
 * @return ESP_OK   transmission succesful
 * @return ESP_FAIL transmission failed
 */
esp_err_t wifi_client_send_data(
    void    *data,
    int     data_size
    );

/**
 * @brief Recieve data from attestation server
 *
 * @param buffer      pointer to buffer containing data to be sent
 * @param buffer_size data buffer size
 *
 * @return ESP_OK   transmission succesful
 * @return ESP_FAIL transmission failed
 */
esp_err_t wifi_client_receive_data(
    void    *buffer,
    int     buffer_size
    );

#endif // WIFI_CLIENT_H