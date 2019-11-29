#include "controller_status.h"

static EventGroupHandle_t controller_event_group;

void controller_status_create()
{
    controller_event_group = xEventGroupCreate();
}

bool controller_status_get_bit(
    int bit
    )
{
    return (xEventGroupGetBits(controller_event_group) & bit) != 0;
}

void controller_status_set_bits(
    int bits
    )
{
    xEventGroupSetBits(controller_event_group, bits);
}

void controller_status_clear_bits(
    int bits
    )
{
    xEventGroupClearBits(controller_event_group, bits);
}

void controller_status_wait_bits(
    int     bits, 
    bool    clear
    )
{
    xEventGroupWaitBits(controller_event_group, bits, clear, false, portMAX_DELAY);
}

esp_err_t controller_status_wait_bits_timeout(
    int     bits, 
    bool    clear,
    int     timeout_ms
    )
{
    EventBits_t event_bits = xEventGroupWaitBits(controller_event_group, bits, clear, false, timeout_ms / portTICK_PERIOD_MS);
    if((event_bits & bits) != bits) 
    {
        // not all bits were set before timeout
        return ESP_ERR_TIMEOUT;
    }
    else 
    {
        return ESP_OK;
    }
}