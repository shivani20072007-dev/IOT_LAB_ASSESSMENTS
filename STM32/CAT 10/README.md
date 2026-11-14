# EXPERIMENT DETAILS

**EXP:** CONTINUOUS ASSESSMENT 10

**DATE:** 05/10/2026

**NAME OF EXP:** STM32–Wemos Wi-Fi Modem Interface Using AT Commands

**HARDWARE SETUP / PLATFORM:** CubeMX + Keil MDK + STM32 Development Board Blue Pill STM32F103C8T6 + Wemos D1 Mini (ESP8266) Wi-Fi Module

---

## TASK 9A — Establish STM32 ↔ Wemos UART Communication

### 1. Circuit Diagram

The experiment interfaces an STM32 Blue Pill development board with a Wemos D1 Mini (ESP8266) Wi-Fi module via UART, alongside a second UART connection to a PC terminal for debugging.

* **Development Board:** STM32 Blue Pill (`STM32F103C8T6`)
* **Wi-Fi Module:** Wemos D1 Mini (ESP8266)
* **UART1:** Connected between STM32 and Wemos D1 Mini for AT commands and responses
* **UART2:** Connected between STM32 and PC Terminal (PuTTY / Tera Term) for debug and display
* **Power & Ground:** Common `GND` reference and appropriate `3.3V` / `5V` supply

| Wemos D1 Mini Pin | STM32 Pin | Description |
| :--- | :--- | :--- |
| **TX** | **RX** (e.g., `PA3` / `USART2_RX`) | Data from Wemos to STM32 |
| **RX** | **TX** (e.g., `PA2` / `USART2_TX`) | Data from STM32 to Wemos |
| **GND** | **GND** | Common Ground Reference |
| **5V / 3.3V** | **5V / 3.3V** | Appropriate Power Source |

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

Before attempting Wi-Fi connectivity, basic serial communication between the STM32 and the ESP8266 modem is verified. The STM32 transmits the basic `AT` test command terminated with Carriage Return and Line Feed (`\r\n`) over UART to the Wemos module, receives the `OK\r\n` response, and forwards the response to the PC serial terminal.

**Pseudocode:**

1. Initialize the HAL library, system clocks, `UART1` (STM32 ↔ Wemos), and `UART2` (STM32 ↔ PC).
2. Transmit `"AT\r\n"` from the STM32 to the Wemos module using `HAL_UART_Transmit()`.
3. Receive the incoming response characters (`'O'`, `'K'`, `'\r'`, `'\n'`) from the Wemos module using `HAL_UART_Receive()`.
4. Forward the received response characters to the PC terminal over the debug UART.

**Code Base:**

Dual-UART communication is implemented using `MX_USART1_UART_Init()`, `MX_USART2_UART_Init()`, `HAL_UART_Transmit()`, and `HAL_UART_Receive()`.

### 3. Inference

The Wemos ESP8266 module responds with `OK` on the PC terminal upon receiving `AT\r\n` from the STM32, confirming proper baud rate matching, cross-wired TX/RX connections, and common ground between the two boards.

---

## TASK 9B — Explore the ESP8266 AT Command Interface

### 1. Circuit Diagram

The hardware connections remain identical to Task 9A, utilizing dual UARTs on the STM32 Blue Pill (`STM32F103C8T6`) connected to the Wemos D1 Mini and PC terminal.

* **UART1:** STM32 ↔ Wemos (AT commands and responses)
* **UART2:** STM32 ↔ PC (Debug / display)

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

The ESP8266 firmware exposes a standardized AT command set over UART to configure Wi-Fi operating modes, scan access points, join networks, obtain IP addresses, and manage TCP connections.

| Requirement | AT Command | Purpose |
| :--- | :--- | :--- |
| Check modem response | `AT` | Verifies serial communication with the ESP8266 |
| Check firmware/version | `AT+GMR` | Displays AT and SDK firmware version info |
| Set Wi-Fi station mode | `AT+CWMODE=1` | Configures ESP8266 as a Wi-Fi client (Station) |
| Scan available networks | `AT+CWLAP` | Lists nearby Wi-Fi Access Points |
| Connect to an AP | `AT+CWJAP="SSID","PASSWORD"` | Connects to the specified Wi-Fi network |
| Obtain network information | `AT+CIFSR` | Queries assigned local IP and MAC addresses |
| Establish TCP connection | `AT+CIPSTART="TCP","host",port` | Opens a TCP socket connection to a server |
| Send application data | `AT+CIPSEND=<length>` | Transmits payload bytes over the active connection |

**Pseudocode:**

1. Initialize both UART interfaces (`STM32 ↔ Wemos` and `STM32 ↔ PC`).
2. Forward commands entered on the PC terminal directly to the Wemos UART interface.
3. Capture the modem's responses and print them to the PC terminal for inspection.

**Code Base:**

Serial passthrough and command testing rely on `HAL_UART_Transmit()` and `HAL_UART_Receive()` across the two configured USART peripherals.

### 3. Inference

Testing the ESP8266 AT command set verifies how the Wi-Fi modem handles configuration, network discovery, IP assignment, and socket operations through structured ASCII commands.

---

## TASK 9C — Connect Wemos to Wi-Fi

### 1. Circuit Diagram

The hardware setup remains the same as in Tasks 9A and 9B, using the STM32 Blue Pill (`STM32F103C8T6`) bridged between the Wemos D1 Mini (`UART1`) and the PC serial terminal (`UART2`).

* **Development Board:** STM32 Blue Pill (`STM32F103C8T6`)
* **Wi-Fi Module:** Wemos D1 Mini (ESP8266)
* **Peripherals:** USB-to-TTL Serial Adapter (Debug Terminal)

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

The STM32 firmware automates the Wi-Fi connection sequence by sending a series of AT commands (`AT`, `AT+CWMODE=1`, `AT+CWJAP`, and `AT+CIFSR`) to the Wemos module, waiting for expected acknowledgments (`OK`, `WIFI CONNECTED`, `WIFI GOT IP`), and reporting the connection status and assigned IP address to the PC terminal.

**Pseudocode:**

1. Initialize the HAL library, `UART1` (Wemos), and `UART2` (PC).
2. Send `"AT\r\n"` to verify modem readiness and wait for `"OK"`.
3. Send `"AT+CWMODE=1\r\n"` to configure Station mode and wait for `"OK"`.
4. Send `"AT+CWJAP=\"MyWiFi\",\"password\"\r\n"` to join the local Wi-Fi Access Point and wait for `"WIFI CONNECTED"`, `"WIFI GOT IP"`, and `"OK"`.
5. Send `"AT+CIFSR\r\n"` to query the assigned IP address.
6. Format and display the verification report (`WIFI MODE : STATION`, `WIFI STATUS : CONNECTED`, `IP ADDRESS : xxx.xxx.xxx.xxx`) on the PC terminal.

**Code Base:**

Automated AT command sequencing and response parsing are implemented using `HAL_UART_Transmit()`, `HAL_UART_Receive()`, `HAL_Delay()`, and standard C string functions (`strstr()`, `memset()`).

### 3. Inference

The STM32 successfully configures the Wemos ESP8266 in Station mode, authenticates with the local Wi-Fi network using AT commands, retrieves its assigned IP address, and displays the live network status on the PC terminal.
