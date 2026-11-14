# EXPERIMENT DETAILS

**EXP:** CONTINUOUS ASSESSMENT 12

**DATE:** 23/10/2026

**NAME OF EXP:** Sensor Interfacing (BME280) – STM32 (Debug) – ESP8266 (Wi-Fi) HTTP – Node-RED Analytics

**HARDWARE SETUP / PLATFORM:** CubeMX + Keil MDK + STM32 Nucleo Development Board + BME280 Environmental Sensor ($I^2C$) + ESP8266 Wi-Fi Module + Node-RED

---

## TASK 12A — BME280 Sensor Interfacing Using $I^2C$ and Bosch Sensor API

### 1. Circuit Diagram

The experiment interfaces a Bosch BME280 environmental sensor with the STM32 Nucleo board over $I^2C$ (`I2C1`), alongside a UART connection for debugging and serial output.

* **Development Board:** STM32 Nucleo Development Board
* **Sensor Module:** Bosch BME280 (Temperature, Pressure, and Humidity)
* **Communication Bus:** $I^2C$ (`I2C1`: `SCL`, `SDA`), Primary 7-bit Address `0x76` (`BME280_I2C_ADDR_PRIM`)
* **Debug Interface:** USART (Serial Terminal)
* **Power & Ground:** `3.3V` and `GND`

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

The STM32 communicates with the BME280 sensor via $I^2C$ using the official Bosch Sensortec BME280 Sensor API (`bme280.c`, `bme280.h`, `bme280_defs.h`). Custom HAL wrapper functions (`user_i2c_read`, `user_i2c_write`, and `user_delay_us`) bind the platform-independent Bosch driver to STM32 `HAL_I2C_Mem_Read()` and `HAL_I2C_Mem_Write()` routines. Upon initialization, the driver verifies the chip ID (`0xD0`), reads factory calibration trim parameters, applies oversampling and filter configurations, and computes compensated temperature (°C), pressure (hPa), and humidity (%RH) values.

**Pseudocode:**

1. Initialize HAL, system clocks, GPIO, $I^2C$ (`MX_I2C1_Init()`), and UART peripherals.
2. Bind `dev.read`, `dev.write`, `dev.delay_us`, and `dev.intf_ptr` (`0x76`) in the `bme280_dev` structure.
3. Call `bme280_init(&dev)` to verify the chip ID and load internal calibration trim registers.
4. Configure temperature, pressure, and humidity oversampling (`1X`), filter coefficient (`OFF`), and power mode (`BME280_MODE_NORMAL`) via `bme280_set_sensor_settings()` and `bme280_set_sensor_mode()`.
5. In the main loop, call `bme280_get_sensor_data(BME280_ALL, &comp_data, &dev)` every 1000 ms to obtain compensated readings.
6. Format and transmit the temperature (°C), pressure (hPa), and humidity (%RH) values over UART.

**Code Base:**

Sensor integration uses the Bosch Sensortec API (`bme280_init()`, `bme280_set_sensor_settings()`, `bme280_set_sensor_mode()`, `bme280_get_sensor_data()`) backed by STM32 HAL functions (`HAL_I2C_Mem_Read()`, `HAL_I2C_Mem_Write()`, `HAL_Delay()`).

### 3. Inference

The STM32 successfully verifies the BME280 chip ID over $I^2C$, initializes the Bosch Sensortec compensation pipeline, and outputs accurate, real-time temperature, atmospheric pressure, and relative humidity readings that respond dynamically to environmental changes.

---

## TASK 12B — ESP8266 Wi-Fi HTTP JSON Server & Node-RED Analytics

### 1. Circuit Diagram

The hardware setup extends Task 12A by connecting the ESP8266 Wi-Fi module to a dedicated UART peripheral on the STM32 Nucleo board to serve live sensor readings over HTTP to a Node-RED analytics flow.

* **Development Board:** STM32 Nucleo Development Board
* **Sensor:** BME280 ($I^2C$)
* **Wi-Fi Module:** ESP8266 (AT Command Firmware over UART)
* **Analytics Client:** Browser / Node-RED Dashboard

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

The STM32 dynamically formats the compensated BME280 readings into a JSON payload, computes the exact byte length (`strlen(json)`), wraps it in a valid `HTTP/1.1 200 OK` response header with `Content-Type: application/json`, and transmits the packet through the ESP8266 using `AT+CIPSEND`. Node-RED periodically polls the HTTP endpoint, parses the JSON telemetry, and computes statistical metrics (Minimum, Maximum, and Average) across the collected samples.

| Parameter | Minimum | Maximum | Average |
| :--- | :--- | :--- | :--- |
| **Temperature (°C)** | 27.91 | 29.02 | 28.46 |
| **Humidity (%RH)** | 59.80 | 64.20 | 62.15 |
| **Pressure (hPa)** | 1007.84 | 1009.31 | 1008.56 |

**Pseudocode:**

1. Acquire compensated `temperature`, `humidity`, and `pressure` values from the BME280 sensor.
2. Construct the JSON string using `snprintf()`:
   `{"status":"success","temperature":XX.XX,"humidity":XX.XX,"pressure":XXXX.XX}`
3. Calculate `content_length = strlen(json)`.
4. Assemble the complete raw HTTP response (`HTTP/1.1 200 OK`, `Content-Type: application/json`, `Content-Length`, followed by `\r\n\r\n` and the JSON body).
5. Send `AT+CIPSEND=<response_length>\r\n` to the ESP8266 and transmit the HTTP payload.
6. In Node-RED, ingest the JSON response over HTTP and compute Minimum, Maximum, and Average statistics for each environmental parameter.

**Code Base:**

Payload formatting and transmission rely on C standard string utilities (`snprintf()`, `strlen()`), STM32 HAL UART routines (`HAL_UART_Transmit()`, `HAL_UART_Receive()`), and ESP8266 AT socket commands (`AT+CIPSEND`).

### 3. Inference

The end-to-end IoT pipeline successfully acquires compensated BME280 sensor data on the STM32, serves structured JSON over HTTP via the ESP8266 Wi-Fi module, and aggregates real-time minimum, maximum, and average environmental statistics in Node-RED.