#ifndef WIFI_STATUS_H
#define WIFI_STATUS_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "types.h"

// Wifi status bits
#define WIFI_CONNECTED_BIT                      BIT0 // Connected to wifi network
#define WIFI_CLIENT_READY_BIT                   BIT1 // Ready to transfer data to server
#define WIFI_CLIENT_DATA_PENDING_BIT            BIT2 // Data is waiting for transfer
#define WIFI_CLIENT_TRANSMISSION_SUCCESS_BIT    BIT3 // Data has been succesfully transferred
#define WIFI_CLIENT_TRANSMISSION_FAIL_BIT       BIT4 // Wifi client failed to transfer data

// Defines used by wifi_status_wait_bits
#define CLEAR       TRUE    // Clear given bits on return
#define DONT_CLEAR  FALSE   // Dont clear given bits on return

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
bool wifi_status_get_bit(
    int bit
    );

/**
 * @brief
 *
 * @param bits
 */
void wifi_status_set_bits(
    int bits
    );

/**
 * @brief
 *
 * @param bits
 */
void wifi_status_clear_bits(
    int bits
    );

/**
 * @brief
 *
 * @param bits  Bits to wait for
 * @param clear if set to CLEAR: set given bits to '0
 *              if set to DONT_CLEAR: dont change given bits  
 */
void wifi_status_wait_bits(
    int bits, 
    bool clear
    );

#endif // WIFI_STATUS_H