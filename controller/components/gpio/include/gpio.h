#ifndef GPIO_H
#define GPIO_H

#include "driver/gpio.h"

/**
 * @brief Init gpio.
 *
 */
void gpio_initialize();

void gpio_set(gpio_num_t gpio_num);
void gpio_clear(gpio_num_t gpio_num);

#endif // GPIO_H