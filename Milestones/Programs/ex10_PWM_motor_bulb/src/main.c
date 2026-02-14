/*
Program 10: Control a motor or light bulb using the PWM output
Description: 
Course: 1064
Author: Daniel Mui
Date: 16/2/2026
 */

 //import tariffs
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/adc.h>

// Create our PWM device
static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(DT_NODELABEL(green_pwm_led));

// Create our ADC device
static const struct device *const adc_dev1 = DEVICE_DT_GET(DT_NODELABEL(adc1));

// Create variable adc_ch1_cfg with the configuration parameters from Overlay
static const struct adc_channel_cfg adc_ch1_cfg = ADC_CHANNEL_CFG_DT(
    DT_CHILD(DT_NODELABEL(adc1), channel_5)
);

int main(void)
{
    uint32_t period = 1000000; 
    uint32_t pulse_width = 0;
    int16_t sample_buffer[1];
    struct adc_sequence sequence = {
        .buffer = sample_buffer,
        .buffer_size = sizeof(sample_buffer),
        .channels = BIT(5),
        .resolution = 12,
    };

    adc_channel_setup(adc_dev1, &adc_ch1_cfg);

    while (1) {
        //Read
        int err = adc_read(adc_dev, &sequence);

        int32_t adc_value = sample_buffer;
        pulse_width = (adc_value * period) / 4095;

        //Write
        pwm_set_dt(&motor, period, pulse_width);

        k_sleep(K_MSEC(50));
    }
    return 0; //we write good code, we return 0
}