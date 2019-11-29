#include "gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "gpio_map.h"

void gpio_initialize()
{
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins to be configured
    io_conf.pin_bit_mask = GPIO_RC522_POWER_SEL | GPIO_LOCK_OPEN_SEL;
    //enable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
}

void gpio_set(gpio_num_t gpio_num) 
{
    gpio_set_level(gpio_num, 1);
}

void gpio_clear(gpio_num_t gpio_num)
{
    gpio_set_level(gpio_num, 0);
}