# EXPERIMENT DETAILS

**EXP:** CONTINUOUS ASSESSMENT 13

**DATE:** 27/10/2026

**NAME OF EXP:** Sensor Interfacing (BME280) – (MQTT) – Node Analytics

**HARDWARE SETUP / PLATFORM:** CubeMX + Keil MDK + STM32 Nucleo Development Board + BME280 Sensor ($I^2C$) + ESP8266 Wi-Fi Module (Custom MQTT API) + Node-RED

---

## BME280 Sensor Interfacing Using $I^2C$ and MQTT via Custom ESP8266 API

### 1. Circuit Diagram

The experiment interfaces a Bosch BME280 environmental sensor over $I^2C$ and an ESP8266 Wi-Fi module over UART (`USART2`) with the STM32 Nucleo development board to publish live telemetry to an MQTT broker and Node-RED dashboard.

* **Development Board:** STM32 Nucleo Development Board
* **Sensor Module:** Bosch BME280 (Temperature, Pressure, and Humidity)
* **Sensor Bus:** $I^2C$ (`I2C1`: `SCL`, `SDA`)
* **Wi-Fi Module:** ESP8266 connected via `USART2` (`huart2`)
* **MQTT Broker & Analytics:** `broker.emqx.io` (Port `1883`) + Node-RED (`mqtt in` node)

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

Building on the previous $I^2C$ BME280 interfacing setup, the STM32 acquires compensated temperature, pressure, and humidity readings via the Bosch Sensortec API (`bme280.c`, `bme280.h`, `bme280_defs.h`) and formats them into a structured JSON payload. Instead of raw HTTP, the firmware uses a custom-built ESP8266 MQTT library (`ESPLib.c` and `ESPLib.h`) to connect to Wi-Fi in Station mode, establish a TCP session with the MQTT broker on port `1883`, and publish the JSON telemetry to a topic (`stm32/test/data`) at 1-second intervals. Node-RED subscribes to the same MQTT topic using the `mqtt in` node to parse and visualize the environmental parameters in real time.

**Pseudocode:**

1. **Driver & Project Setup:**
   * Add `bme280.c` and `ESPLib.c` to `Core/Src`, and `bme280.h`, `bme280_defs.h`, and `ESPLib.h` to `Core/Inc`.
   * Configure `I2C1`, `USART2` (with interrupt reception enabled via `HAL_UART_Receive_IT()`), and `SysTick_Handler` (`MQTTTimer()`).
2. **Initialization (`USER CODE BEGIN 2`):**
   * Initialize the BME280 sensor over $I^2C$ using `bme280_init(&dev)` and set oversampling/normal operating mode.
   * Initialize the ESP8266 in Station mode on `USART2` using `ESP8266_Init(StationMode, huart2)`.
   * Connect to the local Wi-Fi network via `ESP8266_wifiConnect("SSID", "PASSWORD")`.
   * Connect to the MQTT broker (`broker.emqx.io`, port `1883`) via `ESP8266_portConnect("TCP", "broker.emqx.io", "1883")`.
   * Configure `MQTT_InitTypeDef_t` parameters (`keepAlive = 500`, `CleanSession`, `clientID`) and connect via `MQTT_connectBroker(&mqttSetting)`.
3. **Continuous Sensing & Publishing (`while(1)`):**
   * Read compensated temperature (°C), humidity (%RH), and pressure (hPa) values via `bme280_get_sensor_data()`.
   * Construct the JSON payload string using `snprintf()`:
     `{"status":"success","temperature":28.45,"humidity":62.30,"pressure":1008.72}`
   * Publish the JSON payload to the MQTT topic using `MQTT_publishTopic("stm32/test/data", json_payload, QoS0)`.
   * Wait `1000` ms using `HAL_Delay(1000)` before the next cycle.
4. **Node-RED Visualization:**
   * Configure an `mqtt in` node in Node-RED connected to `broker.emqx.io:1883` subscribed to the topic, parse the incoming JSON object, and display temperature, humidity, and pressure charts/gauges.

**Code Base:**

* **Bosch BME280 API:** `bme280_init()`, `bme280_set_sensor_settings()`, `bme280_set_sensor_mode()`, `bme280_get_sensor_data()`
* **Custom ESP8266 & MQTT API (`ESPLib`):** `ESP8266_Init()`, `ESP8266_wifiConnect()`, `ESP8266_portConnect()`, `MQTT_connectBroker()`, `MQTT_publishTopic()`, `MQTT_subscribeTopic()`, `MQTTDataHandler()`, `MQTTTimer()`
* **STM32 HAL & C Utilities:** `HAL_I2C_Mem_Read()`, `HAL_I2C_Mem_Write()`, `HAL_UART_Receive_IT()`, `HAL_Delay()`, `snprintf()`

### 3. Inference

The STM32 successfully acquires compensated BME280 environmental data over $I^2C$, packages the measurements into a JSON string, and publishes them over Wi-Fi to an MQTT broker using the custom `ESPLib` driver, enabling lightweight, real-time telemetry ingestion and visualization in Node-RED.
