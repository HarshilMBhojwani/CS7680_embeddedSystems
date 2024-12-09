/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include <stdio.h>

int main() {

    //setting the value to pico deafualt value for red
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;

// setting another green led
    const uint greenLed=16;

//Initiializing both the pins
    gpio_init(LED_PIN);
    gpio_init(greenLed);

//Making both pins as output
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_dir(greenLed, GPIO_OUT);
    stdio_init_all();

//Making them blink alternate by setting their states alternate
    while (true) {
        gpio_put(LED_PIN, 1);
        gpio_put(greenLed, 0);
        printf("On_board_light_on\n");
        sleep_ms(1000);
        gpio_put(LED_PIN, 0);
        gpio_put(greenLed, 1);
        printf("On_board_light_off\n");
        sleep_ms(1000);
    }
}
