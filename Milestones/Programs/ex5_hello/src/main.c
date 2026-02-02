/*
Program 5: Drive a seven-segment display to display HELLO
Description: 
Course: 1064
Author: Daniel Mui
Date: 2/2/2026
 */

 /*import tarrifs */
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* innit */
static const struct device *gpio_port_a = DEVICE_DT_GET(DT_NODELABEL(gpioa));

static const uint8_t hello_chars[5] = {
    0x76, // H
    0x79, // E
    0x38, // L
    0x38, // L
    0x3F  // O
};

int main(void)
{
    /* Check if GPIOA is ready */
    if (!device_is_ready(gpio_port_a)) {
        return 0;
    }

    /* Configure pins PA0 - PA7 as output */
    for (int pin = 0; pin < 8; pin++) {
        gpio_pin_configure(gpio_port_a, pin, GPIO_OUTPUT_ACTIVE | GPIO_OUTPUT_LOW);
    }

    while (1) {
        /* Loop through the HELLO array */
        for (int i = 0; i < 5; i++) {
            
            /* Write the character to the port */
            gpio_port_set_masked(gpio_port_a, 0xFF, hello_chars[i]);
            
            /* Wait for 1 second */
            k_sleep(K_MSEC(1000));
        }
    }
    return 0; //good code writing frfr
}