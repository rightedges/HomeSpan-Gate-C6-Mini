# Quick Start Guide - HomeSpan Gate Controller (ESP32-C6 Mini)

## What You Need

### Hardware
- ✅ **QSZNTEC ESP32-C6 Mini**
- ✅ **HFD4/3-S Relay Module**
- ✅ **Magnetic Reed Switch** + Magnet
- ✅ **LM2596** DC-DC Step Down Converter
- ✅ **Waterproof Outdoor Antenna**

### Software
- ✅ **Arduino IDE 2.0+**
- ✅ **ESP32 Board Support v3.0.0+**
- ✅ **HomeSpan Library v2.1.0+**

## 5-Minute Setup

### 1. Arduino IDE Setup
1. Open **File → Preferences** and Add:
   `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
2. Open **Tools → Board → Boards Manager**, search for **"esp32"**, and install **v3.0.0 or later**.
3. Go to **Sketch → Include Library → Manage Libraries**, search for **"HomeSpan"**, and install the latest version.

### 2. Board Configuration
1. Connect your ESP32-C6 Mini.
2. Select **Tools → Board → ESP32C6 Dev Module**.
3. Select the correct **Port** under **Tools → Port**.

### 3. Upload Code
1. Open `HomeSpan_Gate/HomeSpan_Gate.ino`.
2. Click **Upload** (→).

### 4. Wiring (ESP32-C6 Mini)
- **Relay (D0)**: GPIO 0
- **Reed Sensor (D8)**: GPIO 8
- **LED (RGB)**: GPIO 15 (Onboard)
- **Button (BOOT)**: GPIO 9 (Onboard)

### 5. Pair with Apple Home
1. Open the **Home** app on your iPhone.
2. Tap **+ → Add Accessory**.
3. Type `W` (if prompted) or enter the pairing code from the Serial Monitor (default is usually `466-37-726`).
4. Assign to "Garage" and rename as "Gate".

## Success! 🎉
Your gate is now ready for use. Enjoy native Siri and HomeKit control.
