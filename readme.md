# Zephyr RTOS Labs – STM32 Nucleo‑L432KC

![Zephyr RTOS](https://img.shields.io/badge/Zephyr-RTOS-blue) ![Board](https://img.shields.io/badge/Board-Nucleo--L432KC-green) ![Course](https://img.shields.io/badge/Course-ENG1064-orange)

This repository contains **incremental lab exercises** for the *Microelectronics 1 (ENG1064)* course. Each example builds on the previous one, targeting the **STM32 Nucleo‑L432KC** using the **Zephyr RTOS** ecosystem.

---

## 📂 Repository Structure

Each exercise is self‑contained and follows the standard Zephyr application structure:

```text
exX_name/
├── boards/
│   └── nucleo_l432kc.overlay   # Hardware / pin configuration (Devicetree)
├── src/
│   └── main.c                  # Application logic
├── prj.conf                    # Kconfig: Zephyr kernel & driver configuration
└── CMakeLists.txt              # Build system instructions
```

---

## 🚀 Exercises & Milestones

### **ex1_blinky — Alternating LEDs**
**Goal:** Flash two external LEDs alternately every 1 second.
*   **Key Concepts:** GPIO output, Devicetree overlays, `gpio_pin_set_dt()`, `k_sleep()`.
*   **Hardware:** External LEDs on **D11 (PB5)** and **D12 (PB4)**.
*   **Milestone:** 1

```c
gpio_pin_set_dt(&red_led, 1);   // Turn RED on
gpio_pin_set_dt(&green_led, 0); // Turn GREEN off
k_msleep(1000);                 // Sleep 1 second
```

---

### **ex2_waves — Dual Square‑Wave Generator**
**Goal:** Generate two simultaneous square waves for oscilloscope observation.
*   **Key Concepts:** Microsecond timing, `k_busy_wait()`, Software frequency division.
*   **Hardware:** 100 kHz on **D10 (PA11)**, 1 kHz on **D9 (PA8)**.
*   **Milestone:** 2

```c
counter++;
if (counter >= 100) {           // Divide 100kHz by 100
    gpio_pin_toggle_dt(&pin_1k); // Toggle 1kHz
    counter = 0; 
}
gpio_pin_toggle_dt(&pin_100k);  // Toggle every 5µs
k_busy_wait(5);                 // Precise timing
```

---

### **ex3_switch_waves — Frequency Selection**
**Goal:** Change the output frequency based on a digital switch input.
*   **Key Concepts:** GPIO input (`gpio-keys`), internal pull-ups, real-time polling.
*   **Logic:** Switch selects between **~0.5 kHz** and **~200 Hz**.
*   **Milestone:** 3, 3b

---

### **ex4_segment_disp — Seven‑Segment (0–9)**
**Goal:** Drive a seven‑segment display using direct MCU port registers.
*   **Key Concepts:** `gpio_port_set_masked()`, Binary-to-Hex mapping, lookup tables.
*   **Hardware:** Common Cathode display on **GPIOA (PA0–PA7)**.
*   **Milestone:** 4

---

### **ex5_hello — Seven‑Segment "HELLO"**
**Goal:** Display alphanumeric characters on a 7-segment display.
*   **Key Concepts:** Custom bitmask patterns for alphabetic characters.
*   **Milestone:** 5

```c
static const uint8_t hello_chars[5] = {
    0x76, // H
    0x79, // E
    0x38, // L
    0x38, // L
    0x3F  // O
};
```

---

### **ex6_dac — Digital‑to‑Analogue Output**
**Goal:** Generate a **100 Hz sawtooth waveform** using the onboard DAC.
*   **Key Concepts:** DAC drivers, 12-bit resolution scaling, voltage references.
*   **Hardware:** Output on PA4 (DAC Channel 1).
*   **Milestone:** 6

---

### **ex7_brightness — LED Bar-Graph**
**Goal:** Map an ADC input voltage to a series of LEDs.
*   **Key Concepts:** ADC sampling, voltage calculation ($V = \text{val} \times \frac{3300}{4095}$), thresholds.
*   **Hardware:** 4 LEDs (D9-D12), Potentiometer/Analog input on **PA0**.
*   **Milestone:** 7

---

### **ex8_LCD_voltage_disp — LCD Voltmeter**
**Goal:** Display live ADC voltage readings on an HD44780 LCD.
*   **Key Concepts:** 4-bit LCD interfacing, `auxdisplay` driver, `snprintf` formatting.
*   **Milestone:** 8

---

### **ex9_PWM_switches — PWM Control**
**Goal:** Adjust PWM frequency and duty cycle using physical switches.
*   **Key Concepts:** Hardware PWM (TIM2), `pwm_set_dt()`.
*   **Parameters:** Period (1 kHz ↔ 1 MHz), Duty Cycle (20% ↔ 80%).
*   **Milestone:** 9

---

### **ex10 — ADC Multi‑Channel Acquisition**
**Goal:** Sample multiple ADC channels sequentially.
*   **Key Concepts:** Multi-channel configuration in Devicetree, sequence buffers.
*   *Note: Under development.*
*   **Milestone:** 10

---

### **ex11 — Timer Interrupts**
**Goal:** Replace busy-waiting with hardware-timed interrupts.
*   **Key Concepts:** Interrupt Service Routines (ISRs), hardware timers, async tasks.
*   *Note: Under development.*
*   **Milestone:** 11

---

### **ex12_i2c_comm — I2C Protocol**
**Goal:** Interface with external I2C sensors or displays.
*   **Key Concepts:** I2C Start/Stop conditions, addressing, bus frequency.
*   *Note: Under development.*
*   **Milestone:** 12

---

## 🛠️ Requirements

- **SDK:** [Zephyr RTOS v3.x.x](https://www.zephyrproject.org/)
- **Toolchain:** [Zephyr SDK](https://docs.zephyrproject.org/latest/develop/toolchains/zephyr_sdk.html) (arm-zephyr-eabi)
- **Board:** [STM32 Nucleo-L432KC](https://www.st.com/en/evaluation-tools/nucleo-l432kc.html)

## 🔨 Building & Flashing

From the root of any exercise folder:

```bash
# Build
west build -b nucleo_l432kc .

# Flash
west flash
```
