#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_log.h"

#include "tasks/task_connection_watcher.h"
#include "wifi_conifg.h"
#include "wifi_status.h"

static const char *LOG_TAG = "task_wifi_connection_watcher";

static esp_err_t status;

void task_wifi_connection_watcher(void *pvParameters)
{
    // wait for connection
    ESP_LOGI(LOG_TAG, "waiting for connection to the wifi network... ");

    wifi_status_wait_bits(WIFI_CONNECTED_BIT);
    ESP_LOGI(LOG_TAG, "connected!\n");

    // print the local IP address
    tcpip_adapter_ip_info_t ip_info;

    status = tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);
    ESP_ERROR_CHECK(status);

    ESP_LOGI(LOG_TAG, "IP Address:  %s\n", ip4addr_ntoa(&ip_info.ip));
    ESP_LOGI(LOG_TAG, "Subnet mask: %s\n", ip4addr_ntoa(&ip_info.netmask));
    ESP_LOGI(LOG_TAG, "Gateway:     %s\n", ip4addr_ntoa(&ip_info.gw));

    while(1)
    {
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}