#include "rfid.h"

#include "string.h" // provides memcpy function

#include "types.h"
#include "gpio_map.h"
#include "rc522.h"
#include "controller_status.h"

static uint8_t card_id_buffer[RFID_TOKEN_LEN_BYTES];

uint8_t* rfid_card_id()
{
    if(controller_status_get_bit(RFID_CARD_DETECTED_BIT) == TRUE)
    {
        return card_id_buffer;
    }
    else
    {
        return NULL;
    }
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
    if(controller_status_get_bit(RFID_CARD_DETECTED_BIT) == FALSE)
    {
        controller_status_set_bits(RFID_CARD_DETECTED_BIT);
        memcpy(card_id_buffer, serial_no, RFID_TOKEN_LEN_BYTES);
    }
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

void rfid_reader_stop()
{
    rc522_pause();
}