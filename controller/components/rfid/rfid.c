#include "rfid.h"

#include "types.h"
#include "gpio_map.h"
#include "rc522.h"
#include "wifi_client.h"

void rfid_reader_stop()
{
    rc522_pause();
}

/**
 * @brief Copy detected card number to external buffer and stop polling
 *  
 * @param serial_no detected card number
 */
static void card_read_callback(
    uint8_t *serial_no
    ) 
{   
    wifi_client_transfer_data(serial_no, RFID_TOKEN_LEN_BYTES);
    rfid_reader_stop();
}

void rfid_reader_start()
{
    const rc522_start_args_t start_args = 
    {
        .miso_io  = GPIO_RC522_SPI_MISO,
        .mosi_io  = GPIO_RC522_SPI_MOSI,
        .sck_io   = GPIO_RC522_SPI_CLK,
        .sda_io   = GPIO_RC522_SPI_CS,
        .callback = &card_read_callback
    };

    rc522_start(start_args);
}