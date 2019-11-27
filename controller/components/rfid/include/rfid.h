#ifndef RFID_H
#define RFID_H

#include "freertos/FreeRTOS.h"

#define RFID_TOKEN_LEN_BYTES 5

esp_err_t rfid_get_tag_id(uint8_t* tag_id);

void rfid_reader_start();

void rfid_reader_stop();

#endif // RFID_H