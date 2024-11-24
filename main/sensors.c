#include <stdio.h>
#include <driver/gpio.h>
#include "sensors.h"

static gpio_num_t pir_gpio, light_gpio;

void sensors_init(gpio_num_t pir_sensor_gpio, gpio_num_t light_sensor_gpio) {
    pir_gpio = pir_sensor_gpio;
    light_gpio = light_sensor_gpio;

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pir_gpio) | (1ULL << light_gpio),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
}

bool is_motion_detected(void) {
    return gpio_get_level(pir_gpio) == 1;
}

bool is_light_detected(void) {
    return gpio_get_level(light_gpio) == 1;
}