
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_log.h"

#include "wifi.h"
#include "wifi_conifg.h"
#include "wifi_status.h"

static const char *LOG_TAG = "wifi";

static esp_err_t status;

static void wifi_event_handler(
    void* 				arg,
    esp_event_base_t 	event_base,
    int32_t 			event_id,
    void* 				event_data
    )
{
    int s_retry_num = 0;

    if (event_base == WIFI_EVENT)
    {
        switch (event_id)
        {
            case WIFI_EVENT_STA_START:
                esp_wifi_connect();
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                if (s_retry_num < WIFI_MAXIMUM_RETRY)
                {
                    esp_wifi_connect();
                    wifi_status_clear_bits(WIFI_CONNECTED_BIT);
                    s_retry_num++;
                    ESP_LOGI(LOG_TAG, "retry to connect to the AP");
                }
                ESP_LOGI(LOG_TAG,"connect to the AP fail");
                break;
        }
    }
    else if (event_base == IP_EVENT)
    {
        switch (event_id)
        {
            case IP_EVENT_STA_GOT_IP:
            {
                ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
                ESP_LOGI(LOG_TAG, "got ip:%s", ip4addr_ntoa(&event->ip_info.ip));
                s_retry_num = 0;
                wifi_status_set_bits(WIFI_CONNECTED_BIT);
            break;
            }
        }
    }
}

// TODO: void or esp_err_t
void wifi_init()
{

    // Create the event group to handle wifi events
    wifi_status_create();

    // Initialize the tcp stack
    tcpip_adapter_init();

    // Initialize the wifi event handler - should this be here or in main?
    status = esp_event_loop_create_default();
    ESP_ERROR_CHECK(status);

    // Initialize the wifi stack
    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    status = esp_wifi_init(&wifi_init_config);
    ESP_ERROR_CHECK(status);

    // Register callbacks to handle wifi events
    status = esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL);
    ESP_ERROR_CHECK(status);

    status = esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL);
    ESP_ERROR_CHECK(status);

    // Wifi config structure
    wifi_config_t wifi_config =
    {
        .sta =
        {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    // Set mode to station
    status = esp_wifi_set_mode(WIFI_MODE_STA);
    ESP_ERROR_CHECK(status);

    // Set wifi config
    status = esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    ESP_ERROR_CHECK(status);

    // wifi_config_t check_config;
    // status = esp_wifi_get_config(ESP_IF_WIFI_STA, &check_config);
    // ESP_ERROR_CHECK(status);

    // ESP_LOGI(LOG_TAG, "ssid: %s", check_config.sta.ssid);
    // ESP_LOGI(LOG_TAG, "pass: %s", check_config.sta.password);
}

void wifi_start()
{
    status = esp_wifi_start();
    ESP_ERROR_CHECK(status);
}