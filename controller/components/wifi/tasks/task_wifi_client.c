#include "tasks/task_wifi_client.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "wifi.h"
#include "tcp_client.h"
#include "tasks/task_connection_watcher.h"


void task_wifi_client(void *pvParameters)
{
    // Initialize WIFI
    // This method has to be called before any tasks that use wifi_status
    wifi_init();

    // Create debug task responsible for wifi status logging
    xTaskCreate(&task_wifi_connection_watcher, "wifi_connection_watcher", 2048, NULL, 5, NULL);

    // Start wifi
    wifi_start();

    // Start tcp client which handles wireless data exchange 
    tcp_client_start();
}