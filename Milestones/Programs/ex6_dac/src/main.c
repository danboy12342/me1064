/*
Program 6: sawtooth wave generator using DAC
Description: 
Course: 1064
Author: Daniel Mui
Date: 2/2/2026
 */

 //import tarrifs
#include <zephyr/kernel.h>
#include <zephyr/drivers/dac.h>

//init
static const struct device *const dac_dev = DEVICE_DT_GET(DT_NODELABEL(dac1));
static const struct dac_channel_cfg dac_ch_cfg = {
    .channel_id = 1,
    .resolution = 12
};

int main(void){
    dac_channel_setup(dac_dev, &dac_ch_cfg);
    //def vars for shape of wave
    int max_val = 3723; // Approx 3.0V for 12-bit DAC with 3.3V reference
    int step_size = 100; // Step size for ramping up
    int sleep_time_us = 270; // Sleep time

    while (1) {
        // Generate the ramp up
        for (int i = 0; i < max_val; i += step_size) {
            dac_write_value(dac_dev, 1, i);
            k_busy_wait(sleep_time_us); 
        }
        // Reset to 0V fro drop in sawtooh
        dac_write_value(dac_dev, 1, 0);
    }
    return 0; /*we write good code here fr fr*/
}