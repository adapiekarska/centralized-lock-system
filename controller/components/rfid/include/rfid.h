#ifndef RFID_H
#define RFID_H

#include "freertos/FreeRTOS.h"

#define RFID_TOKEN_LEN_BYTES 5

uint8_t* rfid_get_tag_id();

void rfid_reader_start();

void rfid_reader_stop();

#endif // RFID_H