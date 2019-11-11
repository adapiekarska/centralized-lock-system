#include "tasks/task_wifi_client.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "wifi.h"
#include "wifi_client.h"


void task_wifi_client(
    void    *pvParameters
    )
{
    // Initialize WIFI
    // This method has to be called before any tasks that use wifi_status
    wifi_init();

    // Start wifi
    wifi_start();

    // Start wifi client which handles wireless data exchange
    wifi_client_start();
}