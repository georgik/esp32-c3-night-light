#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "leds.h"
#include "sensors.h"

#define PIR_SENSOR_GPIO GPIO_NUM_3
#define LIGHT_SENSOR_GPIO GPIO_NUM_4

void app_main(void) {
    // Initialize LEDs
    leds_init(GPIO_NUM_1);

    // Initialize sensors
    sensors_init(PIR_SENSOR_GPIO, LIGHT_SENSOR_GPIO);

    while (true) {
        if (is_motion_detected() && !is_light_detected()) {
            printf("Motion detected, turning on light...\n");
            leds_set_brightness(255); // Turn on LED
        } else {
            printf("No motion or light detected, turning off light...\n");
            leds_set_brightness(50); // Turn off LED
        }
        vTaskDelay(pdMS_TO_TICKS(500)); // Delay for stability
    }
}