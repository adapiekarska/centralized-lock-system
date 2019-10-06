#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_event.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "nvs_flash.h"

#include "wifi.h"

#include "tasks/task_connection_watcher.h"
#include "tasks/task_tcp_client.h"

#define GPIO_EXT_WAKE_PIN GPIO_NUM_26
#define DEEP_SLEEP_WAKE_REASON_PIR ESP_SLEEP_WAKEUP_EXT0

#define HIGH 1

static const char *LOG_TAG = "main";

/**
 * @brief Configure deep sleep
 *
 */
void deep_sleep_init()
{
    esp_err_t ret;

    // Configure ext0 deep sleep wake-up source
    // Only GPIOs which are have RTC functionality can be used: 0,2,4,12-15,25-27,32-39
    ret = esp_sleep_enable_ext0_wakeup(GPIO_EXT_WAKE_PIN, HIGH);
    ESP_ERROR_CHECK(ret);

    // Enable pull-down on wake-up source pin
    ret = rtc_gpio_pulldown_en(GPIO_EXT_WAKE_PIN);
    ESP_ERROR_CHECK(ret);
}

/**
 * @brief Enter deep sleep
 *
 * Never returns
 */
void deep_sleep_start()
{
    esp_deep_sleep_start();
}

/**
 * @brief Configure NVS flash
 *
 */
void nvs_init()
{
    esp_err_t ret;

    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void app_main()
{

    ESP_LOGI(LOG_TAG, "===============SYSTEM RESET===============");

    nvs_init();

    if (esp_sleep_get_wakeup_cause() == DEEP_SLEEP_WAKE_REASON_PIR)
    {
        // Wake up from deep sleep caused by PIR
        ESP_LOGI(LOG_TAG, "Reset from deep sleep");

        // Create default event loop
        // The default event loop is a special type of loop used for system events (WiFi events, for example).
        esp_err_t status = esp_event_loop_create_default();
        ESP_ERROR_CHECK(status);

        // Initialize WIFI
        // This method has to be called before any tasks that use wifi_status
        wifi_init();

        // Create WIFI-related tasks
        xTaskCreate(&task_wifi_connection_watcher, "wifi_connection_watcher", 2048, NULL, 5, NULL);
        xTaskCreate(&task_tcp_client, "tcp_client", 4096, NULL, 5, NULL);

        // Start wifi
        wifi_start();

        // TODO: Wake the RFID reader

        // At the end of this flow, configure and enter deep sleep
        //deep_sleep_init();
        //deep_sleep_start();
    }
    else
    {
        // Not a deep sleep reset
        ESP_LOGI(LOG_TAG, "Normal reset");

        // Configure and enter deep sleep
        deep_sleep_init();
        deep_sleep_start();
    }

}
