#pragma once

#include <stdint.h>
#include <driver/gpio.h>

void leds_init(gpio_num_t gpio_num);
void leds_set_brightness(uint8_t brightness);
