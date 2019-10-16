#include "flow_controller.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

#include "types.h"
#include "gpio_map.h"

#include "controller_status.h"
#include "tasks/task_wifi_client.h"
#include "tasks/task_rfid.h"
#include "rfid.h"
#include "wifi_client.h"

void deep_sleep_wakeup_flow()
{
    static const char *LOG_TAG = "sleep wakeup flow";
    // Wake up from deep sleep caused by PIR
    ESP_LOGI(LOG_TAG, "Reset from deep sleep");

    // Create default event loop
    // The default event loop is a special type of loop used for system events (WiFi events, for example).
    esp_err_t status = esp_event_loop_create_default();
    ESP_ERROR_CHECK(status);

    // Initialize controller status
    controller_status_create();

    // Create Wifi client task
    xTaskCreate(&task_wifi_client, "wifi_client", 4096, NULL, 5, NULL);

    // Wake the RFID reader
    xTaskCreate(&task_rfid, "rfid task", 4096, NULL, 5, NULL);

    // Wait for the rfid to notify about detected card
    ESP_LOGI(LOG_TAG, "Waiting for card");
    controller_status_wait_bits(RFID_CARD_DETECTED_BIT, DONT_CLEAR);
    // Retrieve detected card id
    uint8_t *detected_card = rfid_card_id();
    ESP_LOGI(LOG_TAG, "Detected card: %x %x %x %x %x", 
        detected_card[0],
        detected_card[1],
        detected_card[2],
        detected_card[3],
        detected_card[4]);
        
    // Stop polling for card
    rfid_reader_stop();

    // Communicate with server    
    status = wifi_client_transfer_data(detected_card, RFID_TOKEN_LEN_BYTES);

    // At the end of this flow, configure and enter deep sleep
    //deep_sleep_init();
    //deep_sleep_start();
}