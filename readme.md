# Zephyr RTOS Labs – STM32 Nucleo‑L432KC

![Zephyr RTOS](https://img.shields.io/badge/Zephyr-RTOS-blue) ![Board](https://img.shields.io/badge/Board-Nucleo--L432KC-green) ![Course](https://img.shields.io/badge/Course-ENG1064-orange)

This repository contains **incremental lab exercises** for the *Digital Electronics & Embedded Systems* course. Each example builds on the previous one and targets the **STM32 Nucleo‑L432KC** using **Zephyr RTOS**.
---
## Repository Structure

Each exercise is self‑contained in its own folder and follows the standard Zephyr application layout as given by Dr Giorgos in lecture 2 or 3?.

Inside each exercise:

```text
exX_name/
├── boards/
│   └── nucleo_l432kc.overlay   # Hardware / pin configuration
├── src/
│   └── main.c                  # Application logic
├── prj.conf                    # Zephyr configuration
└── CMakeLists.txt              # Build system
```
---
### **ex1_blinky — Alternating LEDs**
**Goal:** Flash two external LEDs alternately every 1 second.

* GPIO outputs using Device Tree overlays
* External LEDs on **D11 (PB5)** and **D12 (PB4)**
* `gpio_pin_set_dt()` and `k_sleep()`

**Code highlights:**
```c
gpio_pin_set_dt(&red_led, 1);   // Turn RED on
gpio_pin_set_dt(&green_led, 0); // Turn GREEN off
k_msleep(1000);                 // Sleep 1 second
```

**Milestone:** 1
---

### **ex2_waves — Dual Square‑Wave Generator**
**Goal:** Generate two simultaneous square waves and observe them on an oscilloscope.

* **100 kHz** on **D10 (PA11)**
* **1 kHz** on **D9 (PA8)**
* Microsecond‑level timing using `k_busy_wait()`
* Software frequency division using counters

**Code highlights:**
```c
counter++;
if (counter >= 100) {           // Divide 100kHz by 100
    gpio_pin_toggle_dt(&pin_1k); // Toggle 1kHz
    counter = 0; 
}
gpio_pin_toggle_dt(&pin_100k);  // Toggle every 5µs
k_busy_wait(5);                 // Precise timing
```
The **1 kHz** signal is derived by counting 100 toggles of the **100 kHz** signal, achieving frequency division entirely in software.

**Milestone:** 2
---

### **ex3_switch_waves — Frequency Selection via Switch**
**Goal:** Change the output frequency based on a digital input.

* GPIO input using `gpio-keys`
* Internal pull‑up / pull‑down resistors
* Switch selects between two output frequencies
* Demonstrates clean digital input handling

**Code highlights:**
```c
button_state = gpio_pin_get_dt(&button);
if (button_state == 1) {        // Button pressed
    k_busy_wait(1000);          // ~0.5 kHz
} else {                         // Button not pressed
    k_busy_wait(2500);          // ~200 Hz
}
gpio_pin_toggle_dt(&scope);     // Toggle output
```
The switch input on **D9** controls timing intervals, allowing frequency switching between **~0.5 kHz** and **~200 Hz** in real-time.

**Milestone:** 3, 3b
---

### **ex4_segment_disp — Seven‑Segment Display (0–9)**
**Goal:** Drive a seven‑segment display directly using MCU registers.

* Direct GPIO port access (`gpio_port_set_masked()`)
* Binary → Hex → Segment mapping
* Displays digits **0–9** repeatedly

**Code highlights:**
```c
static const uint8_t digits[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    // ... segments for digits
};
for (int i = 0; i < 10; i++) {
    gpio_port_set_masked(gpio_port_a, 0xFF, digits[i]);
    k_sleep(K_MSEC(1000)); // Display each digit for 1 second
}
```
Uses a **lookup table** with hex codes representing segment patterns (a–g) for each digit, written directly to **GPIOA (PA0–PA7)**.

**Milestone:** 4, 4==
---

### **ex5_hello — Seven‑Segment HELLO**
**Goal:** Display the letters **H‑E‑L‑L‑O** on a seven‑segment display.

* Builds on `ex4_segment_disp`
* Custom segment patterns for letters
* Reinforces register‑level GPIO control

**Code highlights:**
```c
static const uint8_t hello_chars[5] = {
    0x76, // H
    0x79, // E
    0x38, // L
    0x38, // L
    0x3F  // O
};
for (int i = 0; i < 5; i++) {
    gpio_port_set_masked(gpio_port_a, 0xFF, hello_chars[i]);
    k_sleep(K_MSEC(1000)); // Display each letter for 1 second
}
```
Extends the digit pattern technique to display custom **alphabetic characters** using segment combinations.

**Milestone:** 5
---

### **ex6_dac — Digital‑to‑Analogue Output**
**Goal:** Generate analogue voltages and waveforms using the onboard DAC.

* Enable `dac1` via Device Tree overlay
* Output fixed voltages (0.5–2.5 V)
* Generate a **100 Hz sawtooth waveform**
* Observe output using an oscilloscope

**Code highlights:**
```c
int max_val = 3723;      // ~3.0V for 12-bit DAC with 3.3V reference
int step_size = 100;
int sleep_time_us = 270;
for (int i = 0; i < max_val; i += step_size) {
    dac_write_value(dac_dev, 1, i);  // Ramp up
    k_busy_wait(sleep_time_us);
}
dac_write_value(dac_dev, 1, 0);      // Reset to 0V
```
Generates a **sawtooth waveform** by ramping the DAC output linearly from 0V to ~3V, then dropping instantly back to 0V, repeating at ~100 Hz.

**Milestones:** 6A, 6B
---

### **ex7_brightness — LED Brightness Control**
**Goal:** Control LED brightness using ADC input voltage levels.

* Four external LEDs on **D9, D10, D11, D12**
* ADC sampling on **channel 5 (PA0)**
* Voltage-to-brightness mapping with thresholds
* Real-time voltage monitoring and LED control

**Code highlights:**
```c
int voltage_mv = buf * 3300 / 4095;  // Convert 12-bit ADC to millivolts
if (voltage_mv < 600) {
    // All LEDs OFF
} else if (voltage_mv < 1200) {
    gpio_pin_set_dt(&led1, 1);  // LED1 ON
} else if (voltage_mv < 1800) {
    gpio_pin_set_dt(&led1, 1);  // LED1, LED2 ON
    gpio_pin_set_dt(&led2, 1);
} // ... more thresholds
```
Maps **ADC voltage** to **LED count**: 4 voltage thresholds progressively illuminate LEDs as input voltage increases, creating a visual bar-graph effect.

**Milestone:** 7
---

### **ex8_LCD_voltage_disp — LCD Voltage Display**
**Goal:** Display ADC voltage measurements on an LCD screen.

* HD44780 LCD display in **4-bit mode**
* ADC sampling on **channel 5 (PA0)**
* Real‑time voltage reading and display
* Converts digital ADC values to readable voltage output

**Code highlights:**
```c
adc_read(adc_dev, &sequence);
int voltage = buf * 3300/4095;  // Convert to millivolts
snprintf(msg1, sizeof(msg1), "Voltage is: %d mV", voltage);
auxdisplay_write(lcd_display, msg1, strlen(msg1));
k_sleep(K_MSEC(200));           // Update every 200ms
```
Reads **ADC channel 5**, converts the 12-bit digital value to voltage, formats as text, and displays on the **LCD** with a 200ms refresh rate.

**Milestone:** 8
---

### **ex9_PWM_switches — PWM Control via Switches**
**Goal:** Control PWM output frequency/duty cycle using physical switches.

* PWM output on **D3 (PA3)** via **TIM2 channel 4**
* Two switch inputs: **D9 (PA8)** and **D10 (PA11)**
* Switch 1 controls **period**: 1 kHz ↔ 1 MHz
* Switch 2 controls **duty cycle**: 20% ↔ 80%

**Code highlights:**
```c
if (switch1_state == 0) {
    period = 1000000;  // 1 MHz
} else {
    period = 1000;     // 1 kHz
}
if (switch2_state == 0) {
    pulse_width = period * 20 / 100;  // 20% duty
} else {
    pulse_width = period * 80 / 100;  // 80% duty
}
pwm_set_dt(&pwm_led0, period, pulse_width);
```
Polls switches every 500ms and dynamically adjusts PWM parameters, allowing independent control of frequency and duty cycle.

**Milestone:** 9
---

### **ex10 — ADC Multi‑Channel Acquisition**
**Goal:** Sample multiple ADC channels simultaneously.

* Configure multiple ADC channels via Device Tree
* Sequential or simultaneous channel sampling
* Display or transmit multi‑channel data
* Demonstrates `adc1` with channels 5+ 

⚠️ *Code under development — currently placeholder only*

**Milestone:** 10
---

### **ex11 — Timer Interrupts**
**Goal:** Implement periodic tasks using hardware timer interrupts.

* Configure and enable hardware timers
* Timer interrupt service routines (ISRs)
* Periodic LED toggling or data sampling
* Demonstrates interrupt‑driven programming patterns

⚠️ *Code under development — currently placeholder only*

Maps **ADC voltage** to **LED display** with thresholds:
- **0–0.6V**: All LEDs off
- **0.6–1.2V**: LED1 on
- **1.2–1.8V**: LED1, LED2 on
- **1.8–2.4V**: LED1, LED2, LED3 on
- **>2.4V**: All LEDs on

Polls ADC every 200ms to update LED states based on input voltage.

**Milestone:** 11
---

### **ex12_I2C_BABY_LETTTTS_GOOOO — I2C Communication**
**Goal:** Communicate with external I2C devices.

* I2C master/slave configuration
* Address selection and data transfer protocols
* Potential sensor or display module communication
* I2C clock and data signal handling

⚠️ *Code under development — currently placeholder only*

**Milestone:** 12
---


