#include <stdio.h>
#include <string.h>
#include <driver/rmt_tx.h>
#include "freertos/FreeRTOS.h"
#include "led_strip_encoder.h"
#include "leds.h"

static rmt_channel_handle_t led_chan = NULL;
static rmt_encoder_handle_t led_encoder = NULL;

#define LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz resolution
#define LED_STRIP_NUM_LEDS      16       // Number of LEDs in the strip

static uint8_t led_strip_pixels[LED_STRIP_NUM_LEDS * 3];

void leds_init(gpio_num_t gpio_num) {
    // Configure RMT TX channel
    rmt_tx_channel_config_t tx_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .gpio_num = gpio_num,
        .resolution_hz = LED_STRIP_RESOLUTION_HZ,
        .mem_block_symbols = 64, // Increased memory to reduce flickering
        .trans_queue_depth = 4,  // Number of queued transmissions
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_config, &led_chan));

    // Configure LED strip encoder
    led_strip_encoder_config_t encoder_config = {
        .resolution = LED_STRIP_RESOLUTION_HZ,
    };
    ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&encoder_config, &led_encoder));

    // Enable the RMT TX channel
    ESP_ERROR_CHECK(rmt_enable(led_chan));
}

void leds_set_brightness(uint8_t brightness) {
    memset(led_strip_pixels, 0, sizeof(led_strip_pixels));
    for (int i = 0; i < LED_STRIP_NUM_LEDS; i++) {
        led_strip_pixels[i * 3 + 0] = brightness;        // Green
        led_strip_pixels[i * 3 + 1] = brightness / 2;    // Blue
        led_strip_pixels[i * 3 + 2] = brightness / 4;    // Red
    }

    // Transmit the updated pixel data
    rmt_transmit_config_t tx_config = {
        .loop_count = 0, // No looping
    };
    ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, led_strip_pixels, sizeof(led_strip_pixels), &tx_config));
    ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY));
}
