/**
MIT License

Copyright (c) 2019 Alija Bobija

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef RC522_H
#define RC522_H

#include "driver/spi_master.h"

typedef void(*rc522_tag_callback_t)(uint8_t*);

typedef struct 
{
    int miso_io;
    int mosi_io;
    int sck_io;
    int sda_io;
    rc522_tag_callback_t callback;
} rc522_start_args_t;

esp_err_t rc522_spi_init(
    int miso_io, 
    int mosi_io, 
    int sck_io, 
    int sda_io
    );

esp_err_t rc522_write_n(
    uint8_t addr, 
    uint8_t n, 
    uint8_t *data
    );

esp_err_t rc522_write(
    uint8_t addr , 
    uint8_t val
    );

uint8_t* rc522_read_n(
    uint8_t addr, 
    uint8_t n
    );

uint8_t rc522_read(
    uint8_t addr
    );

#define rc522_fw_version() rc522_read(0x37)

esp_err_t rc522_init();

esp_err_t rc522_set_bitmask(
    uint8_t addr, 
    uint8_t mask
    );

esp_err_t rc522_clear_bitmask(
    uint8_t addr, 
    uint8_t mask
    );

esp_err_t rc522_antenna_on();

uint8_t* rc522_calculate_crc(
    uint8_t *data, 
    uint8_t n
    );

uint8_t* rc522_tag_write(
    uint8_t cmd, 
    uint8_t *data, 
    uint8_t n, 
    uint8_t* res_n
    );

uint8_t* rc522_request(
    uint8_t* res_n
    );

uint8_t* rc522_anticoll();


uint8_t* rc522_get_tag();

esp_err_t rc522_start(
    rc522_start_args_t start_args
    );

esp_err_t rc522_resume();

esp_err_t rc522_pause();

#endif