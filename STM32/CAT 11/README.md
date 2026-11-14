# EXPERIMENT DETAILS

**EXP:** CONTINUOUS ASSESSMENT 11

**DATE:** 14/10/2026

**NAME OF EXP:** STM32F401RE Cortex-M4: GPIO and User Input Programming

**HARDWARE SETUP / PLATFORM:** CubeMX + Keil MDK + STM32 Nucleo Development Board (NUCLEO-F401RE)

---

## PREREQUISITE & BOARD SPECIFICATIONS

* **Microcontroller IC:** STM32F401RET6 (ARM Cortex-M4 with FPU, up to 84 MHz system clock)
* **Development Board:** STM32 Nucleo-64 (`NUCLEO-F401RE` / `MB1136`)
* **On-Board Oscillator:** 8 MHz HSE bypass clock from ST-LINK/V2-1 (or 16 MHz internal HSI)
* **Peripherals Summary:** Up to 81 I/O ports (GPIOA–GPIOH), 3 USARTs, 3 I2Cs, 4 SPIs, 12-bit ADC, and General-Purpose/Advanced Timers
* **Pinout Reference:** STM32 Nucleo-64 User Manual (`UM1724`)

---

## TASK 1 — On-board LED Bring-Up

### 1. Circuit Diagram

The experiment uses the integrated user LED on the STM32 Nucleo-F401RE development board.

* **Development Board:** STM32 Nucleo-64 (`NUCLEO-F401RE`)
* **Microcontroller IC:** STM32F401RET6
* **GPIO Pin:** `PA5` (LD2 User Green LED, active-high)
* **Peripheral:** On-board User LED (`LD2`)

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

The experiment verifies board bring-up, clock configuration, and basic digital GPIO output control on the `NUCLEO-F401RE` using the STM32 HAL library. Pin `PA5` is configured as a push-pull digital output to blink the green user LED (`LD2`) at a fixed time interval.

**Pseudocode:**

1. Initialize the HAL library and configure system clocks via `SystemClock_Config()`.
2. Initialize GPIO peripheral (`MX_GPIO_Init()`) with `PA5` configured as a digital output.
3. Toggle the logic state of `PA5` (or write `GPIO_PIN_SET` followed by `GPIO_PIN_RESET`).
4. Introduce a fixed millisecond delay.
5. Repeat continuously inside the main `while(1)` loop.

**Code Base:**

GPIO output control is implemented using `HAL_GPIO_TogglePin()` (or `HAL_GPIO_WritePin()`) along with `HAL_Delay()`.

### 3. Inference

The on-board green LED (`LD2` on `PA5`) blinks continuously at a steady interval, confirming proper clock tree setup, ST-LINK flashing, and GPIO digital output operation on the STM32F401RE.

---

## TASK 2 — On-board Switch Control with State Remembrance

### 1. Circuit Diagram

The hardware setup uses the integrated user push-button (`B1`) and user LED (`LD2`) on the `NUCLEO-F401RE` board without external wiring.

* **Development Board:** STM32 Nucleo-64 (`NUCLEO-F401RE`)
* **Microcontroller IC:** STM32F401RET6
* **Input Pin:** `PC13` (B1 Blue User Push-Button, active-low with pull-up)
* **Output Pin:** `PA5` (LD2 User LED)

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

The firmware polls the on-board blue user push-button (`PC13`) and detects valid press events rather than continuous logic levels. When a button press transition is confirmed after software debouncing, the firmware toggles the output state of `PA5` and retains that latched state after the button is released.

**Pseudocode:**

1. Initialize the HAL library, system clocks, `PC13` as digital input, and `PA5` as digital output.
2. Read the current state of `PC13` using `HAL_GPIO_ReadPin()`.
3. When a press transition (`GPIO_PIN_RESET`) is detected, wait for a short debounce interval (e.g., 20–50 ms).
4. Re-verify that `PC13` is still pressed, toggle `PA5` using `HAL_GPIO_TogglePin()`, and wait until the button is released.
5. Maintain the updated LED state until the next valid press occurs.

**Code Base:**

Edge-triggered button reading, debouncing, and latched LED toggling are implemented using `HAL_GPIO_ReadPin()`, `HAL_Delay()`, and `HAL_GPIO_TogglePin()`.

### 3. Inference

Each press of the on-board user button (`B1`) toggles the green LED (`LD2`) cleanly and preserves its ON or OFF state after release, demonstrating edge detection and software switch debouncing.

---

## TASK 3 — Short-Press and Long-Press Detection

### 1. Circuit Diagram

The hardware configuration remains identical to Task 2, utilizing `PC13` (`B1` User Button) and `PA5` (`LD2` User LED) on the `NUCLEO-F401RE`.

* **Development Board:** STM32 Nucleo-64 (`NUCLEO-F401RE`)
* **Microcontroller IC:** STM32F401RET6
* **Input Pin:** `PC13` (User Push-Button)
* **Output Pin:** `PA5` (User LED)

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

The firmware measures how long the user push-button (`PC13`) remains pressed using the non-blocking millisecond tick counter (`HAL_GetTick()`). Based on the elapsed press duration (`release_time - start_time`), the application classifies the input as either a **short press** ($< 1000\text{ ms}$) or a **long press** ($\ge 1000\text{ ms}$) and executes distinct LED behaviors for each action.

| User Action | Press Duration | System Response (`PA5` LED) |
| :--- | :--- | :--- |
| **Short Press** | $< 1\text{ second}$ | Toggles LED ON/OFF (or stops blinking) |
| **Long Press** | $\ge 1\text{ second}$ | Enters continuous non-blocking LED blinking mode |

**Pseudocode:**

1. Initialize the HAL library, system clocks, GPIO pins (`PC13`, `PA5`), and mode state variables.
2. When `PC13` transitions to pressed, record `start_time = HAL_GetTick()` without blocking program execution.
3. When `PC13` is released, record `release_time = HAL_GetTick()` and calculate `duration = release_time - start_time`.
4. Classify the press duration:
   * If `duration < 1000` ms (Short press): Exit blinking mode if active, or toggle `PA5` directly.
   * If `duration >= 1000` ms (Long press): Activate continuous blinking mode.
5. While in blinking mode, compare `HAL_GetTick()` against the last toggle timestamp to blink `PA5` concurrently while monitoring `PC13`.

**Code Base:**

Non-blocking press duration measurement and concurrent LED timing use `HAL_GetTick()`, `HAL_GPIO_ReadPin()`, `HAL_GPIO_WritePin()`, and `HAL_GPIO_TogglePin()`.

### 3. Inference

Using `HAL_GetTick()` allows the STM32F401RE to measure button press durations without blocking the main loop, enabling a single user button (`PC13`) to trigger separate actions for short and long presses.
