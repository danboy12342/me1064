# Zephyr RTOS Labs – STM32 Nucleo‑L432KC

![Zephyr RTOS](https://img.shields.io/badge/Zephyr-RTOS-blue) ![Board](https://img.shields.io/badge/Board-Nucleo--L432KC-green) ![Course](https://img.shields.io/badge/Course-ENG1064-orange)

This repository contains **incremental lab exercises** for the *Digital Electronics & Embedded Systems* course. Each example builds on the previous one and targets the **STM32 Nucleo‑L432KC** using **Zephyr RTOS**.

The focus is on practical MCU I/O: GPIO, timing, digital inputs, direct register access, displays, and DAC output.

---

## Repository Structure

Each exercise is self‑contained in its own folder and follows the standard Zephyr application layout.

```text
.
├── ex1_blinky
├── ex2_waves
├── ex3_switch_waves
├── ex4_segment_disp
├── ex5_hello
├── ex6_dac
```

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

## Exercise Overview

### **ex1_blinky — Alternating LEDs**
**Goal:** Flash two external LEDs alternately every 1 second.

* GPIO outputs using Device Tree overlays
* External LEDs on **D11 (PB5)** and **D12 (PB4)**
* `gpio_pin_set_dt()` and `k_sleep()`

**Milestone:** 1

---

### **ex2_waves — Dual Square‑Wave Generator**
**Goal:** Generate two simultaneous square waves and observe them on an oscilloscope.

* **100 kHz** on **D10 (PA11)**
* **1 kHz** on **D9 (PA8)**
* Microsecond‑level timing using `k_busy_wait()`
* Software frequency division using counters

**Milestone:** 2

---

### **ex3_switch_waves — Frequency Selection via Switch**
**Goal:** Change the output frequency based on a digital input.

* GPIO input using `gpio-keys`
* Internal pull‑up / pull‑down resistors
* Switch selects between two output frequencies
* Demonstrates clean digital input handling

**Milestone:** 3

---

### **ex4_segment_disp — Seven‑Segment Display (0–9)**
**Goal:** Drive a seven‑segment display directly using MCU registers.

* Direct GPIO port access (`gpio_port_set_masked()`)
* Binary → Hex → Segment mapping
* Displays digits **0–9** repeatedly

**Milestone:** 4

---

### **ex5_hello — Seven‑Segment HELLO**
**Goal:** Display the letters **H‑E‑L‑L‑O** on a seven‑segment display.

* Builds on `ex4_segment_disp`
* Custom segment patterns for letters
* Reinforces register‑level GPIO control

**Milestone:** 5

---

### **ex6_dac — Digital‑to‑Analogue Output**
**Goal:** Generate analogue voltages and waveforms using the onboard DAC.

* Enable `dac1` via Device Tree overlay
* Output fixed voltages (0.5–2.5 V)
* Generate a **100 Hz sawtooth waveform**
* Observe output using an oscilloscope

**Milestones:** 6A, 6B

---

## Hardware Used

* **MCU:** STM32 Nucleo‑L432KC
* LEDs + resistors (50–220 Ω)
* Toggle switch
* Seven‑segment display (common cathode)
* Breadboard & jumper wires
* Oscilloscope

---

## Notes

* Pins are **never hard‑coded** in `main.c`; all hardware mapping is done via overlays.
* Each exercise is designed to be demonstrated independently.
* Timing‑critical code avoids scheduler‑based delays where required.

---

Happy hacking 🛠️

