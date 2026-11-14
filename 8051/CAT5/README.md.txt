# EXPERIMENT DETAILS

**EXP:** CONTINUOUS ASSESSMENT 5

**DATE:** 10/08/2026

**NAME OF EXP:** Hardware Timer and Interrupt-Based Timing for LED Fading, Pulse Generation, and Stepper Motor Control

**HARDWARE SETUP / PLATFORM:** Keil µVision + ISP Programmer + 8051 Development Board (`AT89S52`, 11.0592 MHz Crystal) + Digital Oscilloscope

---

## TASK 1 — Hardware Timer 0 Interrupt Time Base (1-Second LED Toggle)

### 1. Circuit Diagram

The experiment configures the internal 16-bit hardware Timer 0 on the 8051 (`AT89S52`) clocked by an 11.0592 MHz crystal oscillator to generate periodic interrupts and toggle an LED output pin without blocking software delay loops.

* **Development Board:** 8051 Development Board
* **Microcontroller IC:** AT89S52 (Classic 12-clock 8051 Core, 11.0592 MHz Crystal)
* **Output Pin:** GPIO pin connected to LED (e.g., `P1^0`)
* **Measurement & Debug:** Digital Oscilloscope and Multimeter

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

For a classic 12-clock 8051 running at $f_{\text{crystal}} = 11.0592\text{ MHz}$, one machine cycle takes $T_{\text{machine}} = \frac{12}{11.0592\text{ MHz}} \approx 1.085\ \mu\text{s}$. To generate a precise $1\text{ ms}$ hardware interrupt, Timer 0 in Mode 1 (16-bit timer) counts $N = \frac{1\text{ ms}}{1.085\ \mu\text{s}} \approx 922$ machine cycles before overflowing, corresponding to a reload value of $65536 - 922 = 64614 = \text{0xFC66}$ (`TH0 = 0xFC`, `TL0 = 0x66`). The Timer 0 ISR increments a software millisecond tick counter and toggles the LED every 1000 ticks ($1\text{ second}$).

**Pseudocode:**

1. Configure `TMOD` for Timer 0 in Mode 1 (16-bit non-auto-reload mode: `TMOD = 0x01`).
2. Load initial timer preload registers `TH0 = 0xFC` and `TL0 = 0x66` for a 1 ms overflow period.
3. Enable Timer 0 interrupt (`ET0 = 1`), global interrupts (`EA = 1`), and start Timer 0 (`TR0 = 1`).
4. Inside `timer0_isr() interrupt 1`:
   * Reload `TH0 = 0xFC` and `TL0 = 0x66`.
   * Increment `ms_ticks`.
   * When `ms_ticks >= 1000`, reset `ms_ticks = 0` and toggle the LED GPIO pin.
5. Keep `main()` non-blocking without software delay loops.

**Code Base:**

Bare-metal SFR configuration using `<reg51.h>` / `<reg52.h>` registers (`TMOD`, `TH0`, `TL0`, `IE` bits `EA` and `ET0`, `TCON` bit `TR0`) and an `interrupt 1` service routine.

### 3. Inference

Timer 0 generates accurate 1 ms periodic hardware interrupts, producing a precise 1-second non-blocking LED toggle verified on the oscilloscope.

---

## TASK 2 — Software PWM LED Fading & Variable-Frequency Signal Generation

### 1. Circuit Diagram

The hardware setup uses the same Timer 0 interrupt time base on the `AT89S52` to drive an LED pin with software PWM and output variable-frequency digital waveforms on a dedicated GPIO signal pin monitored via an oscilloscope.

* **Microcontroller IC:** AT89S52 (11.0592 MHz Crystal)
* **PWM LED Output Pin:** GPIO LED Pin (e.g., `P1^0`)
* **Waveform Signal Output Pin:** GPIO Signal Pin (e.g., `P1^1`)
* **Measurement Instrument:** Digital Oscilloscope

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

Using high-frequency Timer 0 interrupts (500 Hz – 1 kHz PWM carrier), the ISR maintains a PWM period counter (`0` to `99`) and compares it against a dynamic `duty_cycle` threshold (`0%` to `100%` and back to `0%`) to create smooth LED fading without visible flicker. Additionally, by adjusting the timer reload interval across modes, the ISR generates square waves (`100 Hz`, `500 Hz`, `1 kHz`, `2 kHz`, `5 kHz`) as well as custom digital patterns (variable duty-cycle wave, pulse train, and burst signal).

| Duty Cycle | LED Brightness |
| :---: | :--- |
| **0%** | OFF |
| **25%** | Dim |
| **50%** | Medium |
| **75%** | Bright |
| **100%** | Fully ON |

| Signal Type / Mode | Output Waveform / Frequency Behavior |
| :--- | :--- |
| **Square Wave (Modes 1–5)** | Alternating `1010101010` at `100 Hz`, `500 Hz`, `1 kHz`, `2 kHz`, and `5 kHz` |
| **Variable Duty-Cycle Wave** | Asymmetric ON/OFF pulse width (`1111000011110000`) |
| **Pulse Train** | Periodic narrow trigger pulses (`100000001000000010000000`) |
| **Burst Signal** | Packetized pulse bursts separated by idle gaps (`1010101010 0000000000 1010101010`) |

**Pseudocode:**

1. Configure Timer 0 interrupt at high tick resolution to establish the software PWM carrier (~500 Hz – 1 kHz).
2. Inside the Timer 0 ISR:
   * Increment `pwm_counter` (`0` to `99`); drive the LED pin HIGH while `pwm_counter < duty_cycle` and LOW otherwise.
   * Periodically step `duty_cycle` smoothly from `0% → 100% → 0%` for continuous LED breathing/fading.
   * For waveform generation, toggle or shift out the active bit sequence (`Square Wave`, `Variable Duty-Cycle`, `Pulse Train`, or `Burst Signal`) on the signal GPIO pin at the selected frequency mode.

**Code Base:**

Interrupt-driven software PWM and waveform synthesis implemented via `<reg51.h>` / `<reg52.h>` SFRs (`TH0`, `TL0`, `TMOD`, `IE`), counter comparators, and bit-mask pattern arrays inside `interrupt 1`.

### 3. Inference

The AT89S52 produces smooth, flicker-free LED brightness fading via interrupt-driven software PWM and generates clean square waves, pulse trains, and burst signals across multiple frequencies as confirmed on the oscilloscope.

---

## TASK 3 — Stepper Motor Speed and Direction Control (Optional)

### 1. Circuit Diagram

Four GPIO output pins from the `AT89S52` connect through a driver stage (e.g., ULN2003 driver array) to the four stator windings of a unipolar stepper motor.

* **Microcontroller IC:** AT89S52
* **Driver IC:** ULN2003 Darlington Transistor Array
* **Control Pins:** Four GPIO output pins (e.g., `P2.0`–`P2.3`)
* **Actuator:** 4-Phase Unipolar Stepper Motor

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

Rather than using blocking delay loops, the Timer 0 interrupt schedules each commutation step of the stepper motor. Advancing forward or backward through the 4-step wave-drive excitation sequence (`1000 → 0100 → 0010 → 0001`) controls rotational direction, while varying the interrupt step interval controls rotational speed (`100 ms/step` Slow, `50 ms/step` Medium, `10 ms/step` Fast).

| Speed Setting | Timer Step Interval | Step Sequence (Forward) |
| :--- | :---: | :---: |
| **Slow** | `100 ms/step` | `1000 → 0100 → 0010 → 0001` |
| **Medium** | `50 ms/step` | `1000 → 0100 → 0010 → 0001` |
| **Fast** | `10 ms/step` | `1000 → 0100 → 0010 → 0001` |

**Pseudocode:**

1. Define the 4-phase stepping lookup array `{0x08, 0x04, 0x02, 0x01}` (`1000`, `0100`, `0010`, `0001`).
2. Inside the 1 ms Timer 0 ISR, increment `step_timer`.
3. When `step_timer` reaches the target speed interval (`100`, `50`, or `10` ms):
   * Reset `step_timer = 0`.
   * Advance `step_index = (step_index + 1) % 4` for clockwise rotation (or decrement modulo 4 for counter-clockwise).
   * Output `step_sequence[step_index]` to the stepper motor GPIO port.

**Code Base:**

Non-blocking stepper commutation implemented using `<reg51.h>` / `<reg52.h>` port registers, Timer 0 `interrupt 1` tick counters, and a 4-element bitmask array.

### 3. Inference

Using Timer 0 interrupts to schedule stator phase commutations provides deterministic speed control (`10 ms/step` to `100 ms/step`) and reversible direction control of the stepper motor without blocking the CPU.