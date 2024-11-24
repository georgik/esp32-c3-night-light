#pragma once

#include <stdbool.h>
#include <driver/gpio.h>

void sensors_init(gpio_num_t pir_sensor_gpio, gpio_num_t light_sensor_gpio);
bool is_motion_detected(void);
bool is_light_detected(void);