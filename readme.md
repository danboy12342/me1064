# Zephyr RTOS Labs - STM32 Nucleo-L432KC

![Zephyr RTOS](https://img.shields.io/badge/Zephyr-RTOS-blue) ![Board](https://img.shields.io/badge/Board-Nucleo--L432KC-green) ![Course](https://img.shields.io/badge/Course-ENG1064-orange)

This repository contains lab exercises for the **Digital Electronics & Embedded Systems** course. The projects are built using **Zephyr RTOS** on the **STM32 Nucleo-L432KC** development board.

The goal of this repo is to demonstrate how to configure GPIOs, use Device Tree Overlays, and generate precise timing signals.

## Structure

The project follows the standard Zephyr application structure.

```text
/your-project-root
├── /boards
│   └── nucleo_l432kc.overlay   # HARDWARE
├── /src
│   └── main.c                  # LOGIC (The C code running on the MCU)
├── prj.conf                    # CONFIG
└── CMakeLists.txt              # BUILD
```

---

## Hardware Setup

*   **Microcontroller:** STM32 Nucleo-L432KC
*   **Components:**
    *   1x Red LED
    *   1x Green LED
    *   2x Resistors (50Ω - 220Ω)
    *   Breadboard & Jumper Wires
    *   Oscilloscope (for Program 2)

---

## Program 1: Alternating LEDs

**Objective:** Flash an external Red LED (Pin D11) and Green LED (Pin D12) alternately every 1 second.

### Wiring
*   **Red LED:** Anode to **D11** (PB5), Cathode to Resistor -> GND.
*   **Green LED:** Anode to **D12** (PB4), Cathode to Resistor -> GND.

### Key Concepts
*   **Device Tree:** We do not hardcode pins in C. We use an overlay file to alias `led-red` to `PB5`.
*   **Active States:** configured as `GPIO_OUTPUT_ACTIVE` (High) or `GPIO_OUTPUT_INACTIVE` (Low).

---

## Program 2: Square Wave Generator

**Objective:** Generate two simultaneous square waves with vastly different frequencies to observe timing behavior on an oscilloscope.
1.  **100 kHz Signal** on Pin **D10** (PA11).
2.  **1 kHz Signal** on Pin **D9** (PA8).

### The Timing Logic
To achieve **microsecond precision**, we cannot use standard sleep functions.
*   **`k_sleep()`**: Relies on the OS scheduler. Minimum sleep is usually 1ms (1000µs). This is **too slow** for a 100kHz signal (10µs period).
*   **`k_busy_wait()`**: Keeps the CPU active in a loop. This provides exact microsecond timing.

**The Math:**
*   Loop runs every **5µs** (toggles D10).
*   Target slow toggle is **500µs** (1kHz).
*   $500 / 5 = 100$.
*   We use a `counter` variable to toggle D9 every 100th loop iteration.

### Wiring for Scope
*   **Probe 1:** Connect to **D9** (and GND clip to GND).
*   **Probe 2:** Connect to **D10** (and GND clip to GND).

---

## Common Troubleshooting

### 1. "DT_ALIAS not found" or Build Errors
If you get errors saying `led_red` or specific nodes are undefined:
*   Make sure your overlay file is in the `boards/` folder.
*   **Crucial:** The filename must match the board identifier exactly.
    *   ✅ Correct: `nucleo_l432kc.overlay`
    *   ❌ Incorrect: `nucleo 1432kc.overlay` (Space and typo)
*   Perform a **Pristine Build** after renaming.

### 2. Frequency is wrong on the Scope
If your frequency is way lower than expected (e.g., 500Hz instead of 100kHz):
*   Check if you are using `k_sleep()` instead of `k_busy_wait()`.
*   Check your oscilloscope horizontal scale (Time/Div).

---