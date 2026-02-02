/*
Program 3: square wave whose frequency depends on the
position of a switch
Description: 
Course: 1064
Author: Daniel Mui
Date: 2/2/2026
 */

 /*import tarrifs */
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* obtain wooden swoard, i mean aliais from overlay */
#define SW_NODE   DT_ALIAS(sw_input)
#define WAVE_NODE DT_ALIAS(wave_out)

/* innit */
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW_NODE, gpios);
static const struct gpio_dt_spec scope  = GPIO_DT_SPEC_GET(WAVE_NODE, gpios);

int main(void)
{
	int ret;
	int button_state;

	/* Digital 10 conf */
	if (!gpio_is_ready_dt(&scope)) return 0;
	ret = gpio_pin_configure_dt(&scope, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) return 0;

	/* 9 confiong */
	if (!gpio_is_ready_dt(&button)) return 0;
	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) return 0;

	while (1) {
		/* what the button say */
		button_state = gpio_pin_get_dt(&button);
		//gpio_pin_toggle_dt(&scope); /* comment out cause this is for LED */

		/* Toggle out */
		gpio_pin_toggle_dt(&scope);

		if (button_state == 1) { /*trying for 0.5khz*/
			k_busy_wait(1000); /*use busy wait for 'accuracy' could use k_sleep(K_MSEC(250)); for led */
		} else { /*trying for 200Hz*/
			k_busy_wait(2500);
		}
	}
	return 0; /*we write good code here*/
}