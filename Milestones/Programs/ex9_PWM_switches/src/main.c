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
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>

//init GPIOs for LED
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_NODELABEL(led1), gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_NODELABEL(led2), gpios);
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(DT_NODELABEL(led3), gpios);
static const struct gpio_dt_spec led4 = GPIO_DT_SPEC_GET(DT_NODELABEL(led4), gpios);

// init ADC device
static const struct device *const adc_dev = DEVICE_DT_GET(DT_NODELABEL(adc1));

//initADC conf
static const struct adc_channel_cfg adc_ch_cfg = ADC_CHANNEL_CFG_DT(
    DT_CHILD(DT_NODELABEL(adc1), channel_5)
);

int main() {
    // wakeup outs
    gpio_pin_configure_dt(&led1, GPIO_OUTPUT_LOW);
    gpio_pin_configure_dt(&led2, GPIO_OUTPUT_LOW);
    gpio_pin_configure_dt(&led3, GPIO_OUTPUT_LOW);
    gpio_pin_configure_dt(&led4, GPIO_OUTPUT_LOW);
    
    // wakeup ADC
    adc_channel_setup(adc_dev, &adc_ch_cfg);
    
    //code from lab manual
    uint16_t buf;
    struct adc_sequence sequence = {
        .channels = BIT(5),        // ch.5
        .buffer = &buf,
        .buffer_size = sizeof(buf),
        .resolution = 12
    };
    
    
    while(1) {
        //read
        adc_read(adc_dev, &sequence);
        
        //milli conversion
        int voltage_mv = buf * 3300 / 4095;
        
        if (voltage_mv < 600) { //if for controling the LEDs based on voltage levels
            // 0V < Vin < 0.6V: All LEDs OFF
            gpio_pin_set_dt(&led1, 0);
            gpio_pin_set_dt(&led2, 0);
            gpio_pin_set_dt(&led3, 0);
            gpio_pin_set_dt(&led4, 0);
        }
        else if (voltage_mv < 1200) {
            // 0.6V < Vin < 1.2V: LED1 ON
            gpio_pin_set_dt(&led1, 1);
            gpio_pin_set_dt(&led2, 0);
            gpio_pin_set_dt(&led3, 0);
            gpio_pin_set_dt(&led4, 0);
        }
        else if (voltage_mv < 1800) {
            // 1.2V < Vin < 1.8V: LED1, LED2 ON
            gpio_pin_set_dt(&led1, 1);
            gpio_pin_set_dt(&led2, 1);
            gpio_pin_set_dt(&led3, 0);
            gpio_pin_set_dt(&led4, 0);
        }
        else if (voltage_mv < 2400) {
            // 1.8V < Vin < 2.4V: LED1, LED2, LED3 ON
            gpio_pin_set_dt(&led1, 1);
            gpio_pin_set_dt(&led2, 1);
            gpio_pin_set_dt(&led3, 1);
            gpio_pin_set_dt(&led4, 0);
        }
        else {
            // Vin > 2.4V: All LEDs ON
            gpio_pin_set_dt(&led1, 1);
            gpio_pin_set_dt(&led2, 1);
            gpio_pin_set_dt(&led3, 1);
            gpio_pin_set_dt(&led4, 1);
        }
        
        k_sleep(K_MSEC(200));
    }
    //we dont write safe code i guess
}