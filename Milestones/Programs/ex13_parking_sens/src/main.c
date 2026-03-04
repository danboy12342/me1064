/*
Program 13: parking sensor
Description: 
Course: 1064
Author: Daniel Mui
Date: 2/3/2026
 */

 //import tarrifs
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/auxdisplay.h>
#include <stdio.h>
#include <string.h>

// init vars from overlay
static const struct gpio_dt_spec trig_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(trig_pin), gpios);
static const struct gpio_dt_spec echo_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(echo_pin), gpios);
static const struct pwm_dt_spec buzzer = PWM_DT_SPEC_GET(DT_NODELABEL(buzzer_pwm));
static const struct device *const lcd_display = DEVICE_DT_GET(DT_NODELABEL(lcd_screen));

//dist function
int distance() {
    uint32_t timer_start;
    uint32_t timer_stop;
    uint32_t timer_value_us;

    /* obtuse set low*/
    gpio_pin_set_dt(&trig_pin, 0);
    k_sleep(K_MSEC(2)); 

    /* Send a 10us pulse */
    gpio_pin_set_dt(&trig_pin, 1);
    k_busy_wait(10);
    gpio_pin_set_dt(&trig_pin, 0);

    /* Wait for echo to go high */
    while (gpio_pin_get_dt(&echo_pin) == 0) {
        // Wait
    }

    /* Start */
    timer_start = k_cycle_get_32();

    /* Wait */
    while (gpio_pin_get_dt(&echo_pin) == 1) {
        // Wait
    }

    /* Stop */
    timer_stop = k_cycle_get_32();

    /* Read */
    timer_value_us = k_cyc_to_us_floor32(timer_stop - timer_start);

    /* Return dist*/
    if (timer_value_us > 23529) {
        return -1; // Out of range
    } else {
        return timer_value_us / 58; // Converts microseconds to cm (stole from internet)
    }
}

int main(void) {
    char lcd_msg[64];
    
    int tone_period = 1000000;       // 1,000,000 ns = 1 ms = 1kHz pitch
    int duty_cycle = tone_period / 2; // 50%

    // Init pins
    gpio_pin_configure_dt(&trig_pin, GPIO_OUTPUT_LOW);
    gpio_pin_configure_dt(&echo_pin, GPIO_INPUT);
    
    auxdisplay_clear(lcd_display);

//main prg

    while (1) {
        int dist = distance();
        
        if (dist < 0) {
            snprintf(lcd_msg, sizeof(lcd_msg), "Range Exceeded  ");
        } else {
            // Note: I added a couple of blank spaces after "cm" to overwrite 
            // leftover characters when moving from "100 cm" down to "9 cm"
            snprintf(lcd_msg, sizeof(lcd_msg), "Dist: %d cm   ", dist); 
        }
        
        /* Disp*/
        auxdisplay_cursor_position_set(lcd_display, AUXDISPLAY_POSITION_ABSOLUTE, 0, 0);
        auxdisplay_write(lcd_display, lcd_msg, strlen(lcd_msg));

        /* Buzzer*/
        if (dist < 0 || dist > 50) {
            // Out of range or safely above 50cm: OFF
            pwm_set_dt(&buzzer, tone_period, 0); 
            k_sleep(K_MSEC(100));
        } 
        else if (dist <= 5) {
            // 5cm or closer: Solid Tone
            pwm_set_dt(&buzzer, tone_period, duty_cycle); 
            k_sleep(K_MSEC(100)); 
        } 
        else {
            // Between 6cm (and 7) and 50cm: Beep
            int beep_delay_ms = dist * 10; // No casting needed anymore!
            
            // ON
            pwm_set_dt(&buzzer, tone_period, duty_cycle);
            k_sleep(K_MSEC(80)); 
            
            // OFF
            pwm_set_dt(&buzzer, tone_period, 0);
            k_sleep(K_MSEC(beep_delay_ms));
        }
    }
}
