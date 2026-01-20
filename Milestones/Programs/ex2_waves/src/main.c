/*
Program 2: Square Wave Generation (1kHz and 100kHz)
Description: This program generates two square wave signals on an embedded board using Zephyr RTOS.
Course: 1064
Author: Daniel Mui
Date: 19/1/2026
 */

// Inncs
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

//define all
#define SLOW_NODE  DT_ALIAS(scope_slow) // D9
#define FAST_NODE  DT_ALIAS(scope_fast) // D10
//define strcuts
static const struct gpio_dt_spec pin_1k   = GPIO_DT_SPEC_GET(SLOW_NODE, gpios);
static const struct gpio_dt_spec pin_100k = GPIO_DT_SPEC_GET(FAST_NODE, gpios);

//main loop
int main(void)
{
	int counter = 0;

	gpio_pin_configure_dt(&pin_1k, GPIO_OUTPUT_ACTIVE);
	gpio_pin_configure_dt(&pin_100k, GPIO_OUTPUT_ACTIVE);

	while (1) {
		
		// --- 100kHz Signal (Pin D10)
		gpio_pin_toggle_dt(&pin_100k);

		// --- 1kHz Signal (Pin D9)
		counter++;
		if (counter >= 100) { //counting to a number that gives the Hz needed based on the base time of 5us
			gpio_pin_toggle_dt(&pin_1k);
			counter = 0; 
		}

		//constant cpu usage means greater precision in timing
		k_busy_wait(5); 
	}
	return 0;
}