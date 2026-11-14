# EXPERIMENT DETAILS

**EXP:** CONTINUOUS ASSESSMENT 3

**DATE:** 27/07/2026

**NAME OF EXP:** Timer-Based LED Pattern Generator and Button Interrupt Controller

**HARDWARE SETUP / PLATFORM:** Keil µVision + ISP Programmer + 8051 Development Board (`AT89S52`)

---

## Four-State LED Control System with Push-Button & Debouncing

### 1. Circuit Diagram

The experiment interfaces four digital output LEDs and one digital input push-button switch with the 8051 (`AT89S52`) development board using direct memory-mapped register access.

* **Development Board:** 8051 Development Board
* **Microcontroller IC:** AT89S52 (8-bit 8051 CPU Core)
* **Output Pins:** Four GPIO pins connected to LEDs (e.g., `P1.1`–`P1.4` / `P1.0`–`P1.3`)
* **Input Pin:** One GPIO pin connected to a Push-Button Switch (e.g., `P2.0` / matrix keypad input)
* **Hardware Debug:** Digital Multimeter

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

Without using high-level HAL libraries, Arduino APIs, or RTOS services, the bare-metal Embedded C firmware implements a four-state finite state machine (`mode` 1 to 4) controlled by a single push-button. Upon reset (`mode = 0`), all four LEDs remain OFF. Each valid button press—verified through software debouncing and release detection so one physical press triggers exactly one state transition—advances the system through four distinct LED patterns.

| Button Press | Operating Mode | LED Pattern Behavior |
| :--- | :--- | :--- |
| **Initial State** | Idle (`Mode 0`) | All LEDs OFF (`0000`) |
| **1st Press** | **Mode 1 — Running LED** | Sequential chaser: `LED1 → LED2 → LED3 → LED4 → LED1 → ...` |
| **2nd Press** | **Mode 2 — Binary Counter** | 4-bit binary count from `0000` to `1111` (`0` to `15`) |
| **3rd Press** | **Mode 3 — Alternating Pattern** | Toggles complementary pairs (`1010` ↔ `0101`) |
| **4th Press** | **Mode 4 — All LED Flash** | Blinks all four LEDs together (`1111` ↔ `0000`) |

**Pseudocode:**

1. Initialize the LED output port to `0x00` (all LEDs OFF) and configure the push-button input pin.
2. Continuously poll the push-button pin inside the main loop (and between pattern steps):
   * When a button press is detected, wait for a short software debounce interval.
   * Re-read the pin to confirm a valid press and wait until the button is released.
   * Increment `mode` (`1 → 2 → 3 → 4 → 1`).
3. Execute the active mode's LED pattern:
   * **Mode 1:** Shift a single active bit across the four LED pins sequentially with a delay.
   * **Mode 2:** Increment a 4-bit counter (`0` to `15`) and output its binary representation to the four LEDs.
   * **Mode 3:** Alternate between complementary bitmasks (`0x0A` / `0x05` or bitwise XOR inversion).
   * **Mode 4:** Alternate between all LEDs ON (`0x0F` / `1111`) and all LEDs OFF (`0x00` / `0000`).

**Code Base:**

Bare-metal GPIO control and state switching use `<reg51.h>` / `<reg52.h>` Special Function Registers (`P1`, `P2`, `sbit`), bitwise shift/XOR operators (`<<`, `^`), software debounce/delay loops, and a `switch-case` state machine.

### 3. Inference

The bare-metal AT89S52 firmware reliably detects single push-button presses with software debouncing and cycles cleanly through the running LED, 4-bit binary counter, alternating pattern, and all-LED flashing modes using direct register manipulation.
