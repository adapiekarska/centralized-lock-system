#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "nvs_flash.h"

#include "wifi.h"

#include "tasks/task_connection_watcher.h"
#include "tasks/task_tcp_client.h"

void app_main()
{

    // initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // initialize WIFI
    wifi_init();

    // create tasks
    xTaskCreate(&task_wifi_connection_watcher, "wifi_connection_watcher", 2048, NULL, 5, NULL);

    wifi_start();

    // BEGIN: temporary
    vTaskDelay(1000);
    // END: temporary

    xTaskCreate(&task_tcp_client, "tcp_client", 4096, NULL, 5, NULL);

}
