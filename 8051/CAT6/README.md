# EXPERIMENT DETAILS

**EXP:** CONTINUOUS ASSESSMENT 6

**DATE:** 17/08/2026

**NAME OF EXP:** Bare-Metal 8051 UART Driver Development and Serial Echo (AT89S52)

**HARDWARE SETUP / PLATFORM:** Keil µVision + ISP Programmer + 8051 Development Board (`AT89S52`, 11.0592 MHz Crystal) + USB-to-TTL Serial Adapter + PC Terminal (PuTTY)

---

## 8051 Bare-Metal UART Configuration, Character Transmission & PuTTY Echo

### 1. Circuit Diagram

The experiment establishes full-duplex serial UART communication between the 8051 (`AT89S52`) microcontroller and a PC serial terminal (`PuTTY`) via a USB-to-TTL serial converter using direct memory-mapped SFR access.

* **Development Board:** 8051 Development Board
* **Microcontroller IC:** AT89S52 (8-bit 8051 Core, 11.0592 MHz Crystal)
* **Serial Pins:** `P3.0` (`RXD`, Pin 10) and `P3.1` (`TXD`, Pin 11) — alternate peripheral functions of Port 3
* **Interface Converter:** USB-to-TTL Serial Adapter (`TXD ↔ RXD`, `RXD ↔ TXD`, Common `GND`)
* **PC Terminal:** PuTTY configured at `9600` baud, 8 data bits, No parity, 1 stop bit (`9600-8-N-1`)

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

Without using high-level HAL libraries, Arduino APIs, or RTOS services, the bare-metal Embedded C driver configures the AT89S52 serial port (`SCON`) and Timer 1 (`TMOD`, `TH1`) to achieve `9600-8-N-1` asynchronous serial communication. Writing `0x50` to `SCON` selects UART Mode 1 (8-bit variable baud rate) and sets `REN = 1` to enable serial reception. Timer 1 is configured in Mode 2 (8-bit auto-reload, `TMOD = 0x20`) and clocked by the `11.0592 MHz` crystal with reload value `TH1 = 0xFD` ($256 - \frac{11.0592\text{ MHz}}{12 \times 32 \times 9600} = 253 = \text{0xFD}$). Data bytes are written to and read from the `SBUF` register, while transmission and reception completion are synchronized via the `TI` and `RI` interrupt flags.

| Stage / Register | Configuration / Value | Purpose |
| :--- | :---: | :--- |
| **Serial Pins (`P3.0`, `P3.1`)** | `RXD` / `TXD` | Dedicated hardware UART alternate functions on Port 3 |
| **`SCON` Register** | `0x50` (`SM1=1`, `REN=1`) | Selects 8-bit UART Mode 1 with variable baud rate and enables receiver |
| **`TMOD` Register** | `0x20` | Configures Timer 1 in Mode 2 (8-bit auto-reload) for baud generation |
| **`TH1` Reload Value** | `0xFD` (`-3`) | Generates `9600 bps` baud rate with an `11.0592 MHz` crystal (`SMOD = 0`) |
| **`SBUF` Register** | Read / Write | Dual-buffered serial data register (separate TX write and RX read buffers) |
| **`TI` & `RI` Flags** | Polled & Cleared in Software | `TI` indicates transmit buffer empty; `RI` indicates received byte ready |

**Pseudocode:**

1. **UART Initialization (`uart_init()`):**
   * Set `TMOD |= 0x20` to configure Timer 1 in 8-bit auto-reload mode.
   * Load `TH1 = 0xFD` for `9600` baud at `11.0592 MHz`.
   * Set `SCON = 0x50` for UART Mode 1 (`8-N-1`) with reception enabled (`REN = 1`).
   * Start Timer 1 by setting `TR1 = 1`.
2. **Transmit Verification (`uart_tx(char ch)`):**
   * Load the character into `SBUF = ch`.
   * Wait until the transmit interrupt flag `TI` becomes `1` (`while (TI == 0);`).
   * Clear `TI = 0` in software for the next transmission.
3. **Receive Character (`uart_rx()`):**
   * Wait until the receive interrupt flag `RI` becomes `1` (`while (RI == 0);`).
   * Read the received byte from `SBUF` into a `char` variable and clear `RI = 0`.
4. **PuTTY Echo Loop (`main()`):**
   * Call `uart_init()`.
   * Continuously wait for an incoming character from PuTTY (`ch = uart_rx()`) and immediately transmit the exact same character back (`uart_tx(ch)`), echoing strings such as `"Hello 8051"` back to the terminal.

**Code Base:**

Bare-metal serial driver implemented using `<reg51.h>` / `<reg52.h>` Special Function Registers (`SCON`, `SBUF`, `TMOD`, `TH1`, `TR1`, `TI`, `RI`) and modular functions `uart_init()`, `uart_tx()`, and `uart_rx()`.

### 3. Inference

The bare-metal AT89S52 UART driver accurately configures `9600-8-N-1` serial communication using Timer 1 auto-reload mode and the `11.0592 MHz` crystal, reliably receiving characters typed in PuTTY and echoing full strings like `"Hello 8051"` back to the PC terminal.
