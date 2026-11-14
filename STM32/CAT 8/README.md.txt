# EXPERIMENT DETAILS

**EXP:** CONTINUOUS ASSESSMENT 8[cite: 4]

**DATE:** 21/09/2026[cite: 4]

**NAME OF EXP:** STM32 GPIO Bring-Up and Switch-Controlled LED[cite: 4, 7]

**HARDWARE SETUP / PLATFORM:** CubeMX + Keil MDK + STM32 Development Board Blue Pill STM32F103C8T6[cite: 4]

---

## TASK 8A — GPIO Input/Output[cite: 7]

### 1. Circuit Diagram[cite: 4]

The experiment interfaces an external push-button switch and an LED with the STM32 Blue Pill development board[cite: 4, 7].

* **Development Board:** STM32 Blue Pill[cite: 4]
* **Microcontroller IC:** STM32F103C8T6[cite: 4]
* **GPIO Pins:** Input pin configured for Push-Button (with Pull-Up/Pull-Down) and Output pin configured for LED (e.g., `PC13` onboard LED or external GPIO pin)[cite: 4, 7]
* **Peripherals:** Push-Button Switch and LED[cite: 7]

![Circuit Diagram](circuit.png)[cite: 5]

### 2. Core Logic / Pseudocode / Code Base[cite: 5, 6]

**Working Principle:**[cite: 5, 6]

The experiment demonstrates basic GPIO digital input reading and digital output control using the STM32 HAL library[cite: 6, 7]. The microcontroller continuously polls the push-button state and drives the LED pin so that the LED directly follows the switch state[cite: 7].

**Pseudocode:**[cite: 5, 6]

1. Initialize the HAL library and configure system clocks[cite: 5, 6].
2. Configure the switch GPIO pin as a digital input and the LED GPIO pin as a digital output[cite: 7].
3. Read the current logic state of the push-button[cite: 8, 9].
4. If the switch is pressed, turn the LED ON; otherwise, turn the LED OFF[cite: 7, 8].
5. Repeat the process continuously inside the main loop[cite: 6].

**Code Base:**[cite: 6]

The GPIO input and output operations are performed using `HAL_GPIO_ReadPin()` to detect the switch state and `HAL_GPIO_WritePin()` to set the LED output state[cite: 8].

### 3. Inference[cite: 6]

The LED directly mirrors the state of the push-button switch in real time, verifying basic GPIO input/output pin configuration and polling using STM32 HAL functions[cite: 6, 7, 8].

---

## TASK 8B — Event-Based Control[cite: 7]

### 1. Circuit Diagram[cite: 4]

The hardware setup uses the same push-button and LED connections on the STM32 Blue Pill (`STM32F103C8T6`)[cite: 4, 7].

* **Development Board:** STM32 Blue Pill[cite: 4]
* **Microcontroller IC:** STM32F103C8T6[cite: 4]
* **Peripherals:** Push-Button Switch and LED[cite: 7]

![Circuit Diagram](circuit.png)[cite: 5]

### 2. Core Logic / Pseudocode / Code Base[cite: 5, 6]

**Working Principle:**[cite: 5, 6]

Instead of level-sensitive polling, the firmware detects edge transitions (from unpressed to pressed) to toggle the LED state once per button press[cite: 7]. Because mechanical switches generate rapid contact bounces during transitions, a software debouncing delay is applied to filter out false triggers[cite: 7].

**Pseudocode:**[cite: 5, 6]

1. Initialize the HAL library and configure GPIO input/output pins[cite: 5, 6, 7].
2. Continuously monitor the push-button pin and compare the current state with the previous state[cite: 8, 9].
3. When a press transition is detected, wait for a short debounce interval (e.g., 20–50 ms)[cite: 7, 9].
4. Re-read the pin to confirm a valid press[cite: 7, 8].
5. Toggle the LED state and wait until the button is released before accepting the next event[cite: 7].

**Code Base:**[cite: 6]

Edge detection and state toggling are implemented using `HAL_GPIO_ReadPin()` for sampling the switch, `HAL_Delay()` (or `HAL_GetTick()`) for software debouncing, and `HAL_GPIO_TogglePin()` to invert the LED state[cite: 7, 8, 9].

### 3. Inference[cite: 6]

Each discrete press of the push-button reliably toggles the LED ON or OFF without multiple unintended triggers, demonstrating edge detection and software switch debouncing[cite: 7].

---

## TASK 8C — Short Press vs Long Press[cite: 7]

### 1. Circuit Diagram[cite: 4]

The hardware setup remains identical, utilizing the STM32 Blue Pill (`STM32F103C8T6`) interfaced with a push-button and an LED[cite: 4, 7].

* **Development Board:** STM32 Blue Pill[cite: 4]
* **Microcontroller IC:** STM32F103C8T6[cite: 4]
* **Peripherals:** Push-Button Switch and LED[cite: 7]

![Circuit Diagram](circuit.png)[cite: 5]

### 2. Core Logic / Pseudocode / Code Base[cite: 5, 6]

**Working Principle:**[cite: 5, 6]

The firmware measures the duration of each button press using a non-blocking millisecond system tick counter rather than a blocking delay[cite: 9]. Press durations under 1 second are classified as a **short press**, while durations of 1 second or longer are classified as a **long press**, switching the LED between static toggle control and continuous non-blocking blinking mode[cite: 7, 9].

| User Action | System Response |
| :--- | :--- |
| **Short press** ($< 1\text{ s}$) | Toggle LED[cite: 7] |
| **Long press** ($\ge 1\text{ s}$) | LED enters continuous blinking mode[cite: 7] |
| **Next short press** | Stop blinking[cite: 7] |
| **Long press again** | Return to blinking mode[cite: 7] |

**Pseudocode:**[cite: 5, 6]

1. Initialize the HAL library, GPIO pins, and state flags (`blink_mode = 0`)[cite: 5, 6, 7].
2. When the button is pressed, record `start_time = HAL_GetTick()` and continue executing the main loop without blocking[cite: 9].
3. When the button is released, record `release_time = HAL_GetTick()` and compute `duration = release_time - start_time`[cite: 9].
4. Classify the event:
   * If `duration < 1000` ms (Short press): If currently blinking, exit blinking mode; otherwise, toggle the LED[cite: 7].
   * If `duration >= 1000` ms (Long press): Enable continuous blinking mode[cite: 7].
5. While in continuous blinking mode, use `HAL_GetTick()` timestamps to toggle the LED periodically without halting button detection[cite: 7, 9].

**Code Base:**[cite: 6]

Non-blocking timing and event classification are achieved using `HAL_GetTick()` to capture press/release timestamps and schedule LED blink intervals, alongside `HAL_GPIO_ReadPin()`, `HAL_GPIO_WritePin()`, and `HAL_GPIO_TogglePin()`[cite: 7, 8, 9].

### 3. Inference[cite: 6]

Using `HAL_GetTick()` enables non-blocking measurement of button press durations, allowing a single push-button to distinguish between short and long presses while concurrently maintaining continuous LED blinking[cite: 7, 9].