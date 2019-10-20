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

#include "types.h"
#include "gpio_map.h"
#include "tasks/task_wifi_client.h"
#include "tasks/task_rfid.h"
#include "wifi_status.h"

#define DEEP_SLEEP_WAKE_REASON_PIR ESP_SLEEP_WAKEUP_EXT0

#define TASK_WIFI_PRIORITY 10
#define TASK_RFID_PRIORITY 5

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

        // Create Wifi client task along with wifi_status
        wifi_status_create();
        xTaskCreate(&task_wifi_client, "wifi_client", 4096, NULL, TASK_WIFI_PRIORITY, NULL);

        // Wake the RFID reader
        xTaskCreate(&task_rfid, "rfid task", 4096, NULL, TASK_RFID_PRIORITY, NULL);

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
