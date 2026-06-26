# 🚗 Automotive Collision & Rollover Detection System

An **Arduino-based emergency response system** designed to detect vehicle accidents (impact or rollover) and instantly transmit the **precise GPS location** to emergency contacts via SMS.

---

## 🚀 Features

* **Dual-Sensor Detection**

  * YL-99 / SW-420 → detects collision (impact)
  * ADXL335 → detects vehicle tilt and rollover

* **Real-time GPS Tracking**

  * Uses NEO-6M module to fetch accurate coordinates

* **Emergency SMS Alerts**

  * Sends Google Maps link + UTC timestamp via SIM800L

* **Visual & Audible Feedback**

  * I2C OLED display shows system status
  * LED + Buzzer provide alert signaling after crash

---

## 🛠 Hardware Components

### 🔹 Microcontroller

* Arduino Uno

### 🔹 Sensors

* ADXL335 (3-Axis Accelerometer)
* YL-99 / SW-420 (Impact Sensor)

### 🔹 Communication Modules

* SIM800L GSM Module (with 2G SIM)
* NEO-6M GPS Module

### 🔹 User Interface

* 0.96" I2C OLED Display (SSD1306)
* Active Buzzer (5V)
* Red LED + 220Ω Resistor

### 🔹 Power Supply

* 20,000 mAh Power Bank
  *(or 12V Car Battery + Buck Converter)*

---

## 🔌 System Circuit Diagram (Wiring Table)

| Component     | Pins      | Arduino Pin | Notes                      |
| ------------- | --------- | ----------- | -------------------------- |
| OLED Display  | SCL / SDA | A5 / A4     | I2C Communication          |
| GPS NEO-6M    | TX / RX   | D9 / D8     | SoftwareSerial (9600 baud) |
| SIM800L       | TX / RX   | D10 / D11   | Use voltage divider on RX  |
| ADXL335       | X, Y, Z   | A0, A1, A2  | Use 3.3V only              |
| Impact Sensor | DO        | D2          | Digital Input              |
| LED           | Anode     | D6          | Use 220Ω resistor          |
| Buzzer        | +         | D7          | Digital Output             |

---

## 🔌 System Circuit Diagram

<img src="images/circuit_diagram.png" width="600">

---

## ⚙️ Calibration Data (ADXL335)

Based on flat surface calibration:

* **Baseline Values**

  * X ≈ 335
  * Y ≈ 340
  * Z ≈ 402

* **Tilt Detection Threshold**

  * Low: 290
  * High: 390

* **Flip Detection**

  * Z < 320 → Rollover detected

---

## 📝 Installation (PlatformIO)

1. Install **VS Code**
2. Install **PlatformIO extension**
3. Create a new project (Board: Arduino Uno)

Add the following to `platformio.ini`:

```ini
lib_deps =
    greiman/SSD1306Ascii @ ^1.3.5
    mikalhart/TinyGPSPlus @ ^1.0.3
```

4. Copy source code into:

```
src/main.cpp
```

5. Update:

```cpp
EMERGENCY_PHONE
```

6. Build and Upload:

* Build → ✔️
* Upload → ➡️

---

## 🔋 Power Management

* **Monitoring Mode:** ~115 mA
* **Alert Mode:** ~157 mA

### Battery Life

* 20,000 mAh Power Bank
  → ~14,000 mAh usable

➡ Provides approximately:

**~121 hours (~5 days)** continuous operation

⚠ Note:

* SIM800L requires **stable 4.0V supply**
* Must support **2A current bursts**

---

## 📩 Sample SMS Output

```
CRITICAL ALERT!
ROLLOVER detected at 13:34:07 UTC

Location:
https://www.google.com/maps?q=11.02345,76.94532
```

---

## 📌 System Workflow

1. Detect impact using YL-99
2. Detect tilt/flip using ADXL335
3. Fetch GPS coordinates
4. Send SMS via SIM800L
5. Activate buzzer + LED alert
6. Display status on OLED

---

## 🔮 Future Enhancements

* ESP8266/ESP32 IoT Dashboard Integration
* Cloud data logging
* Real-time vehicle tracking
* Emergency call feature
* Accident severity analysis

---

## 👨‍💻 Author

Developed as an **IoT-based Smart Vehicle Safety System** to improve emergency response time and save lives.

---
