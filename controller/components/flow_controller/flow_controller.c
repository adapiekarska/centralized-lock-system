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
#include "gpio.h"

void deep_sleep_wakeup_flow()
{
    static const char *LOG_TAG = "sleep wakeup flow";
    // Wake up from deep sleep caused by PIR
    ESP_LOGI(LOG_TAG, "Reset from deep sleep");

    // Create default event loop
    // The default event loop is a special type of loop used for system events (WiFi events, for example).
    esp_err_t status = esp_event_loop_create_default();
    ESP_ERROR_CHECK(status);

    gpio_initialize();
    // Power-on the RFID chip
    gpio_set(GPIO_RC522_POWER);

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

    // Wait for the rfid to notify about detected tag
    ESP_LOGI(LOG_TAG, "Waiting for tag");
    status = controller_status_wait_bits_timeout(
        RFID_TAG_RETRIEVED_BIT, 
        DONT_CLEAR, 
        RFID_TAG_RETRIEVED_TIMEOUT
        );
    if (status == ESP_ERR_TIMEOUT)
    {
        // no tag detected before timeout
        // log error and finish flow
        ESP_LOGE(LOG_TAG, "TIMEOUT while waiting for tag");
        return;
    }

    // Retrieve detected tag id
    uint8_t *detected_tag = rfid_get_tag_id();
    ESP_ERROR_CHECK(detected_tag == NULL ? ESP_FAIL : ESP_OK);
    ESP_LOGI(LOG_TAG, "Detected tag: %x %x %x %x %x", 
        detected_tag[0],
        detected_tag[1],
        detected_tag[2],
        detected_tag[3],
        detected_tag[4]);
        
    // Stop polling for tag
    rfid_reader_stop();

    // Communicate with server    
    status = wifi_client_send_data(detected_tag, RFID_TOKEN_LEN_BYTES);
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