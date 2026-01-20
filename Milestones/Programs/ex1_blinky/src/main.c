/*
Program 1: Switching on and off a pair of LEDs
Description: This program toggles a red and green LED on and off alternately every second.
It uses the Zephyr RTOS GPIO API to control the LEDs defined in the device tree.
Course: 1064
Author: Daniel Mui
Date:  19/1/2026
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* 
 * Get the node identifiers using the ALIASES defined in the overlay.
 * If the overlay is not found, these lines will cause a build error.
 */
#define RED_NODE    DT_ALIAS(led_red)
#define GREEN_NODE  DT_ALIAS(led_green)

/*
 * Create the GPIO structs
 */
static const struct gpio_dt_spec red_led = GPIO_DT_SPEC_GET(RED_NODE, gpios);
static const struct gpio_dt_spec green_led = GPIO_DT_SPEC_GET(GREEN_NODE, gpios);

int main(void)
{
	int ret;

	/* Check if devices are ready */
	if (!gpio_is_ready_dt(&red_led)) {
		return 0;
	}
	if (!gpio_is_ready_dt(&green_led)) {
		return 0;
	}

	/* Configure Pins */
	ret = gpio_pin_configure_dt(&red_led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) { return 0; }
	
	ret = gpio_pin_configure_dt(&green_led, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) { return 0; }

	while (1) {
		/* Red ON, Green OFF */
		gpio_pin_set_dt(&red_led, 1);
		gpio_pin_set_dt(&green_led, 0);
        printf("Red LED: ON | Green LED: OFF\n");
		k_msleep(SLEEP_TIME_MS);

		/* Red OFF, Green ON */
		gpio_pin_set_dt(&red_led, 0);
		gpio_pin_set_dt(&green_led, 1);
        printf("Red LED: OFF | Green LED: ON\n");
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}