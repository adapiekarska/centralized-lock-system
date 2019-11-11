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
    xTaskCreate(
        &task_wifi_client, 
        "wifi_client", 
        4096, 
        NULL, 
        TASK_WIFI_PRIORITY, 
        NULL
        );

    // Wake the RFID reader
    xTaskCreate(
        &task_rfid, 
        "rfid task", 
        4096, 
        NULL, 
        TASK_RFID_PRIORITY, 
        NULL
        );

    // Wait for the rfid to notify about detected card
    ESP_LOGI(LOG_TAG, "Waiting for card");
    status = controller_status_wait_bits_timeout(
        RFID_CARD_HANDLING_IN_PROGRESS_BIT, 
        DONT_CLEAR, 
        RFID_CARD_HANDLING_IN_PROGRESS_TIMEOUT
        );
    if (status == ESP_ERR_TIMEOUT)
    {
        // no card detected before timeout
        // log error and finish flow
        ESP_LOGE(LOG_TAG, "TIMEOUT while waiting for card");
        return;
    }

    // Retrieve detected card id
    uint8_t *detected_card = rfid_get_card_id();
    ESP_ERROR_CHECK(detected_card == NULL ? ESP_FAIL : ESP_OK);
    ESP_LOGI(LOG_TAG, "Detected card: %x %x %x %x %x", 
        detected_card[0],
        detected_card[1],
        detected_card[2],
        detected_card[3],
        detected_card[4]);
        
    // Stop polling for card
    rfid_reader_stop();

    // Communicate with server    
    status = wifi_client_send_data(detected_card, RFID_TOKEN_LEN_BYTES);
    if (status != ESP_OK){
        // TODO: handle ESP_FAIL and ESP_ERR_TIMEOUT separately
        ESP_LOGE(LOG_TAG, "Failed to send data");
        return;
    }
    char received_bytes[1];
    status = wifi_client_receive_data(received_bytes, 1);
    if (status != ESP_OK){
        // TODO: handle ESP_FAIL and ESP_ERR_TIMEOUT separately
        ESP_LOGE(LOG_TAG, "Failed to receive data");
        return;
    }
    ESP_LOGI(LOG_TAG, "GOT DATA: %d", received_bytes[0]);
    ESP_LOGI(LOG_TAG, "SERVER RESPONSE: %s", received_bytes[0] == 1 ? "OK" : "NOK");
}