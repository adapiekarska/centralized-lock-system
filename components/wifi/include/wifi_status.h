#ifndef WIFI_STATUS_H
#define WIFI_STATUS_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

// Wifi status bits
#define WIFI_CONNECTED_BIT BIT0


/**
 * @brief
 *
 */
void wifi_status_create();

/**
 * @brief
 *
 * @param bit
 * @return true
 * @return false
 */
bool wifi_status_get_bit(int bit);

/**
 * @brief
 *
 * @param bits
 */
void wifi_status_set_bits(int bits);

/**
 * @brief
 *
 * @param bits
 */
void wifi_status_clear_bits(int bits);

/**
 * @brief
 *
 * @param bits
 */
void wifi_status_wait_bits(int bits);

#endif // WIFI_STATUS_H