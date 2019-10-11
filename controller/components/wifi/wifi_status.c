#include "include/wifi_status.h"

static EventGroupHandle_t wifi_event_group;

void wifi_status_create()
{
    wifi_event_group = xEventGroupCreate();
}

bool wifi_status_get_bit(int bit)
{
    //TODO implement this
    return false;
}

void wifi_status_set_bits(int bits)
{
    xEventGroupSetBits(wifi_event_group, bits);
}

void wifi_status_clear_bits(int bits)
{
    xEventGroupClearBits(wifi_event_group, bits);
}

void wifi_status_wait_bits(int bits, bool clear)
{
    xEventGroupWaitBits(wifi_event_group, bits, clear, true, portMAX_DELAY);
}