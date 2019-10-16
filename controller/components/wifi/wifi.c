#include "wifi.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_log.h"

#include "types.h"
#include "wifi_conifg.h"
#include "controller_status.h"

#include "string.h" // think about writing our own strcmp if it's just for this one function

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
                    controller_status_clear_bits(WIFI_CONNECTED_BIT);
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
                controller_status_set_bits(WIFI_CONNECTED_BIT);
            break;
            }
        }
    }
}

static void wifi_configure_sta()
{
    // Set mode to station
    status = esp_wifi_set_mode(WIFI_MODE_STA);
    ESP_ERROR_CHECK(status);

    wifi_config_t wifi_config =
    {
        .sta =
        {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    // Set wifi config
    status = esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    ESP_ERROR_CHECK(status);
}

// TODO: void or esp_err_t
void wifi_init()
{
    // Initialize the tcp stack
    tcpip_adapter_init();

    // Initialize the wifi stack
    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    status = esp_wifi_init(&wifi_init_config);
    ESP_ERROR_CHECK(status);

    // Register callbacks to handle wifi events
    status = esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL);
    ESP_ERROR_CHECK(status);

    status = esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL);
    ESP_ERROR_CHECK(status);

    // If the Wi-Fi NVS flash is enabled by menuconfig, all Wi-Fi configuration in this phase, or later phases,
    // will be stored into flash. When the board powers on/reboots, you do not need to configure the Wi-Fi driver
    // from scratch. You only need to call esp_wifi_get_xxx APIs to fetch the configuration stored in flash previously.
    // You can also configure the Wi-Fi driver if the previous configuration is not what you want.
    // (from https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/wifi.html)

    if (CONFIG_ESP32_WIFI_NVS_ENABLED == TRUE)
    {
        wifi_config_t check_config;
        status = esp_wifi_get_config(ESP_IF_WIFI_STA, &check_config);

        if (status == ESP_OK)
        {
            // Config was read from NVS
            ESP_LOGI(LOG_TAG, "WiFi config was read from NVS flash");

            // If settings have changed, set them again
            if (strcmp((char*)check_config.sta.ssid, WIFI_SSID) != 0 ||
                strcmp((char*)check_config.sta.password, WIFI_PASS) != 0)
            {
                wifi_configure_sta();
            }
        }
        else if (status == ESP_ERR_WIFI_NOT_INIT)
        {
            // WiFi is not initialized by esp_wifi_init, need to initialize
            ESP_LOGI(LOG_TAG, "WiFi config was not present in NVS. Need to initialize");

            wifi_configure_sta();
        }
        else
        {
            // Some error, assert
            ESP_ERROR_CHECK(status);
        }
    }
    else
    {
        // NVS flash not enabled for wifi
        ESP_LOGI(LOG_TAG, "NVS flash for WiFi not enabled");
        wifi_configure_sta();
    }
}

void wifi_start()
{
    status = esp_wifi_start();
    ESP_ERROR_CHECK(status);
}