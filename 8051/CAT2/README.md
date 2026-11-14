# EXPERIMENT DETAILS

**EXP:** CONTINUOUS ASSESSMENT 2

**DATE:** 20/07/2026

**NAME OF EXP:** 8051 Tool Chain Setup & Bare-Metal GPIO LED Control (AT89S52)

**HARDWARE SETUP / PLATFORM:** Keil µVision + ISP Programmer + 8051 Development Board (`AT89S52`)

---

## Hardware Board Bring-Up & Bare-Metal LED Blink

### 1. Circuit Diagram

The experiment performs hardware bring-up on an 8051 (`AT89S52`) development board and interfaces an on-board user LED via direct memory-mapped GPIO register access.

* **Development Board:** 8051 Development Board
* **Microcontroller IC:** AT89S52 (8-bit 8051 CPU Core)
* **Operating Voltage:** 5V DC
* **Programming / Debug Interface:** ISP (In-System Programming) Programmer
* **Peripheral:** User LED connected to GPIO Port (e.g., Port 1 / Port 2)

| Parameter | Entry |
| :--- | :--- |
| **MCU** | AT89S52 |
| **CPU Core** | 8-bit 8051 Architecture |
| **Operating Voltage** | 5V |
| **Debug Interface** | ISP Programmer |
| **LED GPIO Port** | Port 1 / Port 2 |
| **LED GPIO Pin** | Pin 0 (e.g., `P1^0` / `P2^0`) |
| **LED Active State** | Active-Low / Active-High (board dependent) |
| **LED Frequency (Hz)** | ~1 Hz |

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

Without using high-level HAL libraries, Arduino APIs, or RTOS services, the bare-metal Embedded C program directly manipulates the 8051 Special Function Registers (SFRs) defined in `<reg51.h>` (or `<reg52.h>`). A custom software delay function based on nested loops is implemented to toggle the target LED GPIO pin at an approximate frequency of 1 Hz (500 ms ON, 500 ms OFF).

**Pseudocode:**

1. **Hardware Bring-Up:**
   * Verify the 5V power rails on the AT89S52 development board using a digital multimeter.
   * Identify the user LED port/pin from the board schematic and establish ISP communication between the host PC and target MCU.
2. **Custom Delay Function (`delay_ms`):**
   * Implement a reusable software delay function using nested `for` loops calibrated to the crystal oscillator frequency (e.g., 11.0592 MHz or 12 MHz).
3. **Bare-Metal LED Blink (`main`):**
   * Configure/access the target LED GPIO pin via bit-addressable SFR (`sbit`) or full port register write.
   * Set the LED pin ON (`0` or `1` depending on active state).
   * Call the custom delay function for ~500 ms.
   * Set the LED pin OFF.
   * Call the custom delay function for ~500 ms.
   * Repeat continuously inside an infinite `while(1)` loop to achieve a ~1 Hz blink rate.

**Code Base:**

Bare-metal register manipulation uses `<reg51.h>` / `<reg52.h>` SFR definitions (`sbit` or direct port assignments like `P1 = 0x00;` / `P1 = 0xFF;`) and a custom loop-based delay function (`void delay_ms(unsigned int ms)`).

### 3. Inference

The AT89S52 board power rails and ISP toolchain were verified, and the bare-metal C program successfully blinked the on-board LED at approximately 1 Hz using direct memory-mapped SFR access and a custom software delay library function.
