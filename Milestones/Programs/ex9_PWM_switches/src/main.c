/*
Program 9: Create a PWM signal with period and duty cycle
controlled by switches
Description: 
Course: 1064
Author: Daniel Mui
Date: 16/2/2026
 */

 //import tarrifs
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>

//mk pwm device for led0
static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(DT_NODELABEL(green_pwm_led));

//consts for switches
static const struct gpio_dt_spec switch1 = GPIO_DT_SPEC_GET(DT_NODELABEL(switch1), gpios);
static const struct gpio_dt_spec switch2 = GPIO_DT_SPEC_GET(DT_NODELABEL(switch2), gpios);

int main(void)
{
    // Confs to mk ins
    gpio_pin_configure_dt(&switch1, GPIO_INPUT);
    gpio_pin_configure_dt(&switch2, GPIO_INPUT);
    //inits
    int period;
    int pulse_width;
    int switch1_state;
    int switch2_state;
    
    while (1) {
        // Read
        switch1_state = gpio_pin_get_dt(&switch1);
        switch2_state = gpio_pin_get_dt(&switch2);
        
        // Switch 1 controls period: 1kHz or 1MHz
        if (switch1_state == 0) {
            period = 1000000;
        } else {ds
            period = 1000;
        }
        
        // Switch 2 controls duty cycle: 20% or 80%
        if (switch2_state == 0) {
            // 20% duty cycle
            pulse_width = period * 20 / 100;
        } else {
            // 80% duty cycle
            pulse_width = period * 80 / 100;
        }

        pwm_set_dt(&pwm_led0, period, pulse_width);
        k_sleep(K_MSEC(500)); //poll rate (but only for swtches, pwm runs indy)
    }
}