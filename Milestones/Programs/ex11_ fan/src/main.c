/*
Program 11: Create a cooling fan

1. Connect Pin-1 to VDD (3.3V) and Pin-3 to GND. Pin-2 should be connected to one of
your MCU’s Analogue-to-Digital Converters (ADC), i.e. PA0.

2. Create an appropriate algorithm to convert the ADC input to temperature, using the
figure above.

3. Wire appropriately your LCD display and output the measured temperature on the
screen.

4. Use a PWM output and control the duty-cycle based on the temperature. We would like
for any temperatures below 35°C that the duty-cycle is 0%, i.e. the pulse width is 0. 
For temperatures >35°C and up to 100°C the duty cycle should change linearly from 
0% to 100%. Use appropriate frequency/period for the PWM (see your lecture notes).

5. Implement a motor (see Milestone 10). You will need a power supply, a transistor, a
diode, a gate resistor and pull-down resistor. Do not turn on the power supply unless
your circuit is checked by your tutor.

6. Use the soldering iron to induce heat on your temperature sensor. Adjust the soldering
iron temperature to 90°C. GENTLY TOUCH THE SOLDERING IRON ON THE SENSOR.
Check on the LCD that the temperature is actually increasing and that once we exceed
35°C your motor starts to work.

7. (Advanced task) Now implement a pull-down switch together with an interrupt, so
that the user can override the system. When the MCU receives logic 1 from the 
pull-down switch, the motor should start working on a 50% duty cycle and for 5 seconds,
no matter what the temperature is. After the 5 seconds elapse, the fan should switch off
and control of the fan should return to the temperature sensor.

Course: 1064
Author: Daniel Mui
Date: 16/2/2026
 */

 //import tarrifs
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/auxdisplay.h>
#include <stdio.h>
#include <string.h>

// Devices<--Overlay
static const struct pwm_dt_spec motor = PWM_DT_SPEC_GET(DT_ALIAS(pwm_motor));
static const struct device *adc_dev = DEVICE_DT_GET(DT_NODELABEL(adc1));
static const struct device *lcd_dev = DEVICE_DT_GET(DT_NODELABEL(lcd_screen));
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_ALIAS(override_btn), gpios);

// ADC Cconf
static const struct adc_channel_cfg channel_5_cfg = {
    .gain = ADC_GAIN_1,
    .reference = ADC_REF_INTERNAL,
    .acquisition_time = ADC_ACQ_TIME_DEFAULT,
    .channel_id = 5, //ch.5
};

int16_t sample_buffer;
struct adc_sequence sequence = {
    .channels = BIT(5),
    .buffer = &sample_buffer,
    .buffer_size = sizeof(sample_buffer),
    .resolution = 12,
};

//interupt variables
volatile bool override_active = false;
int64_t override_end_time = 0;
struct gpio_callback button_cb_data;

// 7
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    override_active = true;
    override_end_time = k_uptime_get() + 5000; // Set timer for 5 seconds
}

int main(void) //main function
{
    adc_channel_setup(adc_dev, &channel_5_cfg);
    auxdisplay_clear(lcd_dev);
    
    // 7
    gpio_pin_configure_dt(&button, GPIO_INPUT);
    gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
    gpio_add_callback(button.port, &button_cb_data);
    char line_buffer[16];
    uint32_t period = 1000000; // 1kHz
    uint32_t pulse_width = 0;

    while (1) { //main loop
        // 1
        adc_read(adc_dev, &sequence);
        int32_t raw_value = sample_buffer;

        // 2
        int32_t voltage_mv = (raw_value * 3300) / 4095;
        int32_t temp_c = voltage_mv / 20;

        // 7
        bool override_now = false;
        if (override_active) {
            if (k_uptime_get() > override_end_time) {
                override_active = false; 
            } else {
                override_now = true;
            }
        }

        if (override_now) {
            // 7
            pulse_width = period / 2;
        } 
        else {
            // 4
            if (temp_c < 35) {
                pulse_width = 0;
            } 
            else if (temp_c >= 100) {
                pulse_width = period;
            } 
            else {
                int32_t temp_offset = temp_c - 35;
                pulse_width = (temp_offset * period) / 65;
            }
        }

        // 5
        pwm_set_dt(&motor, period, pulse_width);

        // 3
        auxdisplay_cursor_position_set(lcd_dev, AUXDISPLAY_POSITION_ABSOLUTE, 0, 0);
        snprintf(line_buffer, sizeof(line_buffer), "Temp: %d C    ", temp_c);
        auxdisplay_write(lcd_dev, (uint8_t *)line_buffer, strlen(line_buffer));

        // 6
        k_sleep(K_MSEC(200));
    }
    return 0; //we write safe code frfr 
}