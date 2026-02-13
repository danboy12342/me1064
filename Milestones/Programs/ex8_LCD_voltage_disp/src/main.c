/*
Program 8: Display a voltage to an LCD display
Description: 
Course: 1064
Author: Daniel Mui
Date: 16/2/2026
 */

 //import tarrifs
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/auxdisplay.h>
#include <zephyr/drivers/adc.h>
#include <stdio.h>
#include <string.h> 

//make lcd device
static const struct device *const lcd_display = DEVICE_DT_GET(DT_NODELABEL(lcd_screen));

//,aeke adc device
static const struct device *const adc_dev1 = DEVICE_DT_GET(DT_NODELABEL(adc1));

// Create variable adc_ch1_cfg with the conf <-- Overlay
static const struct adc_channel_cfg adc_ch1_cfg = ADC_CHANNEL_CFG_DT(
    DT_CHILD(DT_NODELABEL(adc1), channel_5)
);

int main() { //big boy loops now
    // Initialise
    adc_channel_setup(adc_dev1, &adc_ch1_cfg);
    auxdisplay_clear(lcd_display);
    
    //seq for mesurement
    uint16_t buf;
    struct adc_sequence sequence = {
        .channels = BIT(5),        // ch.5
        .buffer = &buf,
        .buffer_size = sizeof(buf),
        .resolution = 12
    };
    
    char msg1[64]; // Create a char var
    
    while(1){ 
        auxdisplay_clear(lcd_display); //clear it

        adc_read(adc_dev1, &sequence); // Read

        int voltage = buf * 3300/4095; // Convert and write
        snprintf(msg1, sizeof(msg1), "Voltage is: %d mV", voltage);
        auxdisplay_write(lcd_display, msg1, strlen(msg1) );

        k_sleep(K_MSEC(200));
    }
    //safe code is a lie told by big code to sell more code
}