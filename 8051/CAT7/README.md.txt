# EXPERIMENT DETAILS

**EXP:** CONTINUOUS ASSESSMENT 7

**DATE:** 24/08/2026

**NAME OF EXP:** 16×2 LCD Driver Development and Dynamic Display Control (AT89S52)

**HARDWARE SETUP / PLATFORM:** Keil µVision + ISP Programmer + 8051 Development Board (`AT89S52`) + HD44780 16×2 Character LCD + 4×4 Matrix Keypad

---

## TASK 7A — LCD Initialization and Character/String Display

### 1. Circuit Diagram

The experiment interfaces an HD44780-compatible 16×2 character LCD with the 8051 (`AT89S52`) microcontroller in **4-bit GPIO mode** using direct memory-mapped register access.

* **Development Board:** 8051 Development Board
* **Microcontroller IC:** AT89S52 (8-bit 8051 CPU Core)
* **Display Module:** 16×2 Character LCD (HD44780 Controller)
* **Control Pins:** `RS` (Register Select), `RW` (Read/Write, tied to `GND` for write-only), and `EN` (Enable strobe) connected to GPIO pins (e.g., `P2.0`, `P2.1`)
* **Data Bus (4-Bit Mode):** Upper nibble data lines `D4`–`D7` connected to GPIO pins (e.g., `P2.4`–`P2.7`)
* **Power & Contrast:** `+5V` VCC, `GND`, and potentiometer contrast adjustment on `V0` (`VEE`)

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

Without using high-level HAL libraries, Arduino APIs, or RTOS services, the bare-metal Embedded C driver initializes the HD44780 LCD controller in 4-bit, 2-line mode (`5×7` dot matrix). Each 8-bit command or ASCII character is split into two 4-bit nibbles (upper nibble first, followed by the lower nibble) and latched into the LCD using a high-to-low enable (`EN`) pulse, with `RS = 0` for commands and `RS = 1` for display data.

| HD44780 Command (Hex) | Function |
| :---: | :--- |
| `0x02` | Return Home / Initialize 4-bit mode |
| `0x28` | 4-bit interface, 2 lines, 5×7 font |
| `0x0C` | Display ON, Cursor OFF |
| `0x06` | Entry mode: auto-increment cursor right |
| `0x01` | Clear display screen |
| `0x80` / `0xC0` | Force cursor to beginning of Row 1 / Row 2 |

**Pseudocode:**

1. **Send Nibble & Enable Pulse:**
   * Place the 4-bit nibble onto `D4`–`D7`, pulse `EN = 1`, wait briefly, and set `EN = 0`.
2. **Command Write (`lcd_cmd(unsigned char cmd)`):**
   * Set `RS = 0` (Command register).
   * Transmit the upper 4 bits (`cmd >> 4`) followed by the lower 4 bits (`cmd & 0x0F`).
3. **Data Write (`lcd_data(unsigned char dat)`):**
   * Set `RS = 1` (Data register).
   * Transmit the upper 4 bits (`dat >> 4`) followed by the lower 4 bits (`dat & 0x0F`).
4. **Initialization & Display (`lcd_init()`):**
   * Wait for LCD power-on stabilization (>15 ms), then send initialization sequence `0x02`, `0x28`, `0x0C`, `0x06`, and `0x01`.
   * Write characters and text strings onto both Row 1 and Row 2.

**Code Base:**

Bare-metal 4-bit LCD driver implemented using `<reg51.h>` / `<reg52.h>` SFRs (`sbit` pins for `RS`, `EN`, `D4`–`D7`) and functions `lcd_init()`, `lcd_cmd()`, and `lcd_data()`.

### 3. Inference

The AT89S52 successfully initializes the HD44780 16×2 LCD in 4-bit mode using only 6 GPIO pins and renders individual characters and multi-character strings cleanly across both display rows.

---

## TASK 7B — Cursor Positioning and Dynamic Value Update

### 1. Circuit Diagram

The hardware connections remain identical to Task 7A, using the `AT89S52` wired to the 16×2 HD44780 LCD in 4-bit mode.

* **Microcontroller IC:** AT89S52
* **Display Module:** HD44780 16×2 LCD (4-bit data bus `D4`–`D7` + `RS`, `EN`)

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

To support dynamic UI updates without clearing the entire screen, the driver computes the exact DDRAM address for any `(row, column)` coordinate (`0x80 + column` for Row 0 and `0xC0 + column` for Row 1). Strings are printed sequentially via pointer iteration, and multi-digit integer values are converted into individual ASCII characters (`'0'`–`'9'`) for live numeric display.

**Pseudocode:**

1. **Cursor Positioning (`lcd_goto(unsigned char row, unsigned char column)`):**
   * If `row == 0`, send command `0x80 + column`.
   * If `row == 1`, send command `0xC0 + column`.
2. **String Output (`lcd_puts(char *string)`):**
   * Loop through each character until the null terminator `'\0'` and call `lcd_data(*string++)`.
3. **Numeric Output (`lcd_print_number(unsigned int value)`):**
   * Handle `0` directly (`lcd_data('0')`).
   * Extract decimal digits (`value % 10`), store them in a temporary buffer, and transmit them in reverse order as ASCII characters (`digit + '0'`).

**Code Base:**

Dynamic display utilities built on top of `lcd_cmd()` and `lcd_data()` using `<reg51.h>` / `<reg52.h>`:

* `void lcd_goto(unsigned char row, unsigned char column)`
* `void lcd_puts(char *string)`
* `void lcd_print_number(unsigned int value)`

### 3. Inference

The cursor-positioning and formatting functions allow the AT89S52 to place text at exact `(row, column)` coordinates and dynamically update changing integer values on the 16×2 LCD without display flicker.

---

## TASK 7C — 4×4 Keypad-Based Character Movement

### 1. Circuit Diagram

The hardware setup extends Tasks 7A and 7B by connecting a 4×4 matrix keypad to an available GPIO port on the `AT89S52` alongside the 16×2 LCD.

* **Microcontroller IC:** AT89S52
* **Display Module:** HD44780 16×2 LCD (4-bit mode)
* **Input Peripheral:** 4×4 Matrix Keypad connected to a GPIO Port (e.g., `P1.0`–`P1.3` rows, `P1.4`–`P1.7` columns)

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

The firmware combines row-column matrix keypad scanning with cursor-addressed LCD rendering to create an interactive 2D cursor-movement application across the 16×2 grid (`row` $\in [0, 1]$, `col` $\in [0, 15]$). Pressing directional keys (`2`, `8`, `4`, `6`) clears the character at the previous `(row, col)` position, updates the coordinates within display bounds, and redraws the character at the new position.

| Keypad Key | Direction | Coordinate Update (`row`, `col`) |
| :---: | :---: | :--- |
| **`2`** | `↑` Move Up | Decrements `row` (`1 → 0`) |
| **`8`** | `↓` Move Down | Increments `row` (`0 → 1`) |
| **`4`** | `←` Move Left | Decrements `col` (`col > 0 ? col - 1 : 0`) |
| **`6`** | `→` Move Right | Increments `col` (`col < 15 ? col + 1 : 15`) |

**Pseudocode:**

1. Initialize the LCD (`lcd_init()`) and keypad GPIO port; draw the initial character at `(row = 0, col = 0)`.
2. Continuously scan the 4×4 keypad with software debouncing and release detection.
3. When directional key `2`, `8`, `4`, or `6` is pressed:
   * Move cursor to current `(row, col)` via `lcd_goto(row, col)` and overwrite with a space `' '` to erase the old position.
   * Update `row` or `col` according to the pressed direction key (clamped within `0..1` and `0..15`).
   * Move cursor to the updated `(row, col)` via `lcd_goto(row, col)` and display the character via `lcd_data()`.

**Code Base:**

Interactive keypad-to-LCD application combining bare-metal matrix scanning (`keypad_scan()`) with `lcd_goto()` and `lcd_data()` using `<reg51.h>` / `<reg52.h>`.

### 3. Inference

The AT89S52 integrates 4×4 matrix keypad scanning with 4-bit LCD cursor addressing to move a character smoothly across both rows and all 16 columns of the display in response to directional key presses.