/*
Program 4: Drive a seven-segment display to display 0-9 repeatedly
position of a switch
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

static const uint8_t digits[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

int main(void)
{
    /* Check if GPIOA is ready */
    if (!device_is_ready(gpio_port_a)) {
        return 0;
    }

    for (int pin = 0; pin < 8; pin++) {
        gpio_pin_configure(gpio_port_a, pin, GPIO_OUTPUT_ACTIVE | GPIO_OUTPUT_LOW);
    }

    while (1) {
        /* Loop through numbers 0 to 9 */
        for (int i = 0; i < 10; i++) {

            gpio_port_set_masked(gpio_port_a, 0xFF, digits[i]);
            
            /* Wait for next number */
            k_sleep(K_MSEC(1000));
        }
    }
    return 0; //good code writing frfr
}