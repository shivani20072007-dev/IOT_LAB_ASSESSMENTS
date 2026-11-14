# EXPERIMENT DETAILS

**EXP:** CONTINUOUS ASSESSMENT 9

**DATE:** 28/09/2026

**NAME OF EXP:** STM32 UART Command Controlled LED

**HARDWARE SETUP / PLATFORM:** CubeMX + Keil MDK + STM32 Development Board Blue Pill STM32F103C8T6 + USB-to-TTL Converter / PC Terminal (PuTTY)

---

## TASK 9A — Basic UART Communication

### 1. Circuit Diagram

The experiment establishes serial UART communication between the STM32 Blue Pill development board and a PC serial terminal, alongside the onboard LED.

* **Development Board:** STM32 Blue Pill
* **Microcontroller IC:** STM32F103C8T6
* **UART Pins:** `PA9` (USART1_TX) and `PA10` (USART1_RX) connected to a USB-to-TTL serial adapter
* **GPIO Pin:** `PC13` (Onboard LED)
* **Peripherals:** USB-to-TTL Converter and Onboard LED

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

The experiment configures the STM32 USART1 peripheral in asynchronous mode (standard baud rate, e.g., 9600 or 115200 bps, 8-N-1) using STM32CubeMX and HAL. In the first stage, the microcontroller continuously transmits a character to the PC terminal. In the second stage, it receives incoming characters from the PC terminal and immediately echoes them back. Additionally, carriage return (`\r`) and line feed (`\n`) formatting behavior is verified on the terminal.

**Pseudocode:**

1. Initialize the HAL library, system clocks, and UART peripheral (`MX_USART1_UART_Init()`).
2. **Transmit Mode:**
   * Load a character buffer and transmit it to the PC terminal using `HAL_UART_Transmit()`.
   * Introduce a delay and repeat continuously.
3. **Receive and Echo Mode:**
   * Wait to receive a 1-byte character from the PC terminal using `HAL_UART_Receive()`.
   * Upon reception, immediately transmit the same byte back to the PC terminal using `HAL_UART_Transmit()`.
4. **Line Ending Test (`\r` vs `\n`):**
   * Transmit `"HELLO\r\n"`, `"HELLO\r"`, `"HELLO\n"`, and `"HELLO\r\nWORLD\r\n"` to observe cursor return (`\r`) and new-line advancement (`\n`) in PuTTY.

**Code Base:**

Serial initialization, transmission, and reception are performed using `HAL_UART_Init()`, `HAL_UART_Transmit()`, and `HAL_UART_Receive()`.

### 3. Inference

Characters transmitted from the STM32 appear reliably on the PuTTY serial terminal, and keystrokes entered in the terminal are echoed back accurately. Testing `\r` and `\n` demonstrates that `\r` (Carriage Return) moves the cursor to the start of the current line, `\n` (Line Feed) moves the cursor down one line, and both `\r\n` are required together in standard serial terminals to start cleanly on a new line.

---

## TASK 9B — String-Based Application Logic

### 1. Circuit Diagram

The hardware setup uses the same USART1 serial interface (`PA9`/`PA10`) and onboard LED (`PC13`) on the STM32 Blue Pill (`STM32F103C8T6`).

* **Development Board:** STM32 Blue Pill
* **Microcontroller IC:** STM32F103C8T6
* **UART Pins:** `PA9` (TX), `PA10` (RX)
* **GPIO Pin:** `PC13` (Onboard LED)
* **Peripherals:** USB-to-TTL Converter and Onboard LED

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

The firmware implements a command-line parser over UART. Incoming characters are buffered until a newline/carriage-return delimiter is received. The assembled string is compared against predefined commands (`LED ON`, `LED OFF`, `LED TOGGLE`) to control the onboard LED on `PC13` and send a status confirmation message back to the terminal.

| Terminal Command | LED Action (`PC13`) | Terminal Response |
| :--- | :--- | :--- |
| `LED ON` | Turns onboard LED ON | `LED turned ON` |
| `LED OFF` | Turns onboard LED OFF | `LED turned OFF` |
| `LED TOGGLE` | Toggles onboard LED state | `LED toggled` |

**Pseudocode:**

1. Initialize the HAL library, GPIO (`PC13` as output), and UART peripheral (`HAL_UART_Init()`).
2. Read incoming bytes sequentially from UART using `HAL_UART_Receive()` and store them in a character buffer until `\r` or `\n` is detected.
3. Null-terminate the received command string.
4. Compare the command string using `strcmp()`:
   * If `"LED ON"`: Set `PC13` to turn the LED ON and transmit `"LED turned ON\r\n"`.
   * If `"LED OFF"`: Reset `PC13` to turn the LED OFF and transmit `"LED turned OFF\r\n"`.
   * If `"LED TOGGLE"`: Toggle `PC13` and transmit `"LED toggled\r\n"`.
5. Clear the receive buffer index and wait for the next command.

**Code Base:**

String reception and command feedback use `HAL_UART_Receive()` and `HAL_UART_Transmit()`, string matching uses standard C `strcmp()`, and LED control uses `HAL_GPIO_WritePin()` and `HAL_GPIO_TogglePin()`.

### 3. Inference

The STM32 successfully parses multi-character string commands sent from a PC serial terminal, controls the onboard LED in real time, and returns confirmation responses over UART.