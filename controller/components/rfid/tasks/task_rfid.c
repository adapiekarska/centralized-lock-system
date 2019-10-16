#include "tasks/task_rfid.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "types.h"
#include "rfid.h"
#include "wifi_status.h"

void task_rfid(
    void *pvParameters
    ) 
{
    rfid_reader_start();
    vTaskDelete(NULL);
}