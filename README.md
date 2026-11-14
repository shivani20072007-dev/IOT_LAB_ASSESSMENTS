# Fundamentals of IoT – Lab Repository

## Overview

This repository holds the lab work and hands-on implementations I completed for the **Fundamentals of Internet of Things (FIoT) Lab** course at **Shiv Nadar University, Chennai**.

Everything is arranged by CAT (Continuous Assessment Test) number, with each CAT folder containing the exercises assigned for that test. The experiments are built on both **8051 and STM32 microcontrollers** and span embedded programming, hardware interfacing, peripheral communication, and IoT applications.

## Repository Layout

```text
FIoT_LAB_25011102086/
│
├── 8051/
│   ├── CAT2/
│   ├── CAT3/
│   ├── CAT4/
│   ├── CAT5/
│   ├── CAT6/
│   └── CAT7/
│
├── STM32/
│   ├── CAT8/
│   ├── CAT9/
│   ├── CAT10/
│   ├── CAT11/
│   ├── CAT12/
│   ├── CAT13/
│   ├── CAT14/
│   └── CAT15/
│
└── README.md
```

Inside every CAT folder, you will find the individual task folders along with their implementation files.

## Platforms, Tools and Technologies

The experiments make use of the following hardware, software, and technologies:

- **8051 microcontroller (AT89S52)**
- **STM32F103C8T6 (Blue Pill)**
- **STM32F401RE Nucleo board**
- STM32CubeMX
- Keil µVision / Keil MDK-ARM
- Embedded C
- STM32 HAL libraries
- GPIO, UART, I2C, and other peripheral interfaces
- ESP8266 Wi-Fi module
- Wi-Fi setup through AT commands
- TCP and HTTP communication
- MQTT communication
- BME280 environmental sensor

## What Each Experiment Includes

Where applicable, every experiment folder provides:

- Embedded C source code
- Project and configuration files for the microcontroller
- Circuit diagrams
- A dedicated README describing the experiment
- Notes on the implementation and the observed results

Experiments are grouped by CAT number, and the separate tasks for each CAT sit inside its own folder.

## Topics Covered

The lab exercises build practical skills in the following areas:

- Programming and interfacing the 8051 microcontroller
- Configuring and controlling GPIO on STM32
- Handling push buttons, debouncing, and driving LEDs
- Serial communication over UART
- Interfacing LCDs and keypads
- Configuring the ESP8266 Wi-Fi module with AT commands
- Setting up a TCP server and handling HTTP requests
- Communicating over MQTT
- Interfacing sensors and collecting environmental data
- Embedded programming with HAL libraries

## Purpose

This repository is a record of my IoT lab experiments, practical implementations, and hardware interfacing exercises from the course.

It shows how microcontrollers, peripheral communication protocols, sensors, wireless connectivity, and IoT communication methods were applied through hands-on practice.
