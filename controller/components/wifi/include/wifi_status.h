#ifndef WIFI_STATUS_H
#define WIFI_STATUS_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "types.h"

// Wifi status bits
#define WIFI_CONNECTED_BIT                      BIT0
#define WIFI_CLIENT_READY_BIT                   BIT1
#define WIFI_CLIENT_DATA_PENDING_BIT            BIT2
#define WIFI_CLIENT_TRANSMISSION_COMPLETE_BIT   BIT3

#define CLEAR       TRUE
#define DONT_CLEAR  FALSE

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
 * @param clear
 */
void wifi_status_wait_bits(int bits, bool clear);

#endif // WIFI_STATUS_H