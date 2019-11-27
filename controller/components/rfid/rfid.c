#include "rfid.h"

#include "string.h" // provides memcpy function

#include "types.h"
#include "gpio_map.h"
#include "rc522.h"
#include "controller_status.h"

static uint8_t tag_id_buffer[RFID_TOKEN_LEN_BYTES];

uint8_t* rfid_get_tag_id()
{
    if(controller_status_get_bit(RFID_TAG_HANDLING_IN_PROGRESS_BIT) == TRUE)
    {
        return tag_id_buffer;
    }
    else
    {
        return NULL;
    }
}

/**
 * @brief Copy detected tag number to external buffer and stop polling
 *  
 * @param serial_no detected tag number
 */
static void tag_read_callback(
    uint8_t *serial_no
    ) 
{   
    if(controller_status_get_bit(RFID_TAG_HANDLING_IN_PROGRESS_BIT) == FALSE)
    {
        controller_status_set_bits(RFID_TAG_HANDLING_IN_PROGRESS_BIT);
        memcpy(tag_id_buffer, serial_no, RFID_TOKEN_LEN_BYTES);
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
        .callback = &tag_read_callback
    };

    rc522_start(start_args);
}

void rfid_reader_stop()
{
    rc522_pause();
}