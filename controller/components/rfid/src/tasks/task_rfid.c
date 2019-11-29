#include "tasks/task_rfid.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "types.h"
#include "rfid.h"
#include "controller_status.h"

void task_rfid(
    void *pvParameters
    ) 
{
    rfid_reader_start();
    controller_status_wait_bits(RFID_TAG_RETRIEVED_BIT, DONT_CLEAR);
    vTaskDelete(NULL);
}