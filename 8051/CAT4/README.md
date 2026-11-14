# EXPERIMENT DETAILS

**EXP:** CONTINUOUS ASSESSMENT 4

**DATE:** 03/08/2026

**NAME OF EXP:** Human-Machine Interface using Matrix Keypad, LEDs, and Seven-Segment Display

**HARDWARE SETUP / PLATFORM:** Keil µVision + ISP Programmer + 8051 Development Board (`AT89S52`)

---

## Embedded User Interface with Matrix Keypad, Seven-Segment Display & LEDs

### 1. Circuit Diagram

The experiment interfaces a matrix keypad (user input), a seven-segment display (visual digit feedback), and four LEDs (operating mode indicators) with the 8051 (`AT89S52`) development board using direct memory-mapped register access.

* **Development Board:** 8051 Development Board
* **Microcontroller IC:** AT89S52 (8-bit 8051 CPU Core)
* **Input Peripheral:** Matrix Keypad (Row/Column GPIO Port, e.g., `P2`)
* **Display Peripheral:** Seven-Segment Display (`a`–`g`, `dp` connected to GPIO Port, e.g., `P0`)
* **Indicator Peripherals:** Four LEDs connected to GPIO Port (e.g., `P1`)
* **Hardware Debug:** Digital Multimeter

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

Without using high-level HAL libraries, Arduino APIs, or RTOS services, the bare-metal Embedded C firmware implements a modular Human-Machine Interface (HMI) on the AT89S52. The microcontroller continuously scans the matrix keypad via row-column scanning with software debouncing. Whenever a valid numeric key (`0`–`9`) is pressed, the firmware updates the seven-segment display using a hex lookup table to show the pressed digit and dispatches the corresponding LED control function or multi-step LED pattern.

| Key Press | Seven-Segment Display | LED Operation |
| :---: | :---: | :--- |
| **1** | `1` | LED1 ON |
| **2** | `2` | LED2 ON |
| **3** | `3` | LED3 ON |
| **4** | `4` | LED4 ON |
| **5** | `5` | Running LED Pattern |
| **6** | `6` | Alternate LEDs (`1010` ↔ `0101`) |
| **7** | `7` | Binary Counter (`0`–`15`) |
| **8** | `8` | All LEDs Blink |
| **9** | `9` | All LEDs OFF |
| **0** | `0` | Reset System |

**Pseudocode:**

1. Initialize GPIO ports for the matrix keypad, seven-segment display, and four indicator LEDs (default state: `0` displayed, LEDs OFF).
2. Continuously scan the matrix keypad using `keypad_scan()`:
   * Sequentially drive each keypad row low and read the column pins.
   * When a low column is detected, apply a software debounce delay and re-verify the key press.
   * Map the `(row, col)` coordinate to the corresponding numeric key value (`0`–`9`).
3. Upon detecting a valid key press:
   * Call `display_digit(value)` to output the seven-segment bit pattern from a lookup array to the display port.
   * Call `led_control(key)` to execute the mapped operation (`LED1`–`LED4` individual ON, `running_led()`, `alternate_led()`, `binary_counter()`, `blink_all()`, all OFF, or system reset).
4. During continuous pattern modes (`5`–`8`), poll `keypad_scan()` between steps so any new key press immediately switches modes.

**Code Base:**

Modular bare-metal implementation using `<reg51.h>` / `<reg52.h>` Special Function Registers and dedicated functions:

* `void keypad_scan(void)` — Row/column matrix scanning with software debouncing
* `void display_digit(unsigned char value)` — Seven-segment lookup table output
* `void led_control(unsigned char key)` — Mode dispatcher for keys `0`–`9`
* `void running_led(void)` — Sequential 4-LED chaser pattern
* `void binary_counter(void)` — 4-bit binary count (`0000`–`1111`)
* `void alternate_led(void)` — Complementary alternating LED toggle (`1010` ↔ `0101`)
* `void blink_all(void)` — Simultaneous all-LED flashing (`1111` ↔ `0000`)

### 3. Inference

The AT89S52 successfully integrates matrix keypad scanning, seven-segment numerical feedback, and multi-mode LED control in a structured, modular bare-metal C program using direct register manipulation.
