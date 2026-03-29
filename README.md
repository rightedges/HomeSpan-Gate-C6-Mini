# HomeSpan Gate Controller (ESP32-C6 Mini)

A HomeKit-native sliding gate controller using the [HomeSpan](https://github.com/HomeSpan/HomeSpan) library for the ESP32-C6. This project provides a seamless, native "Garage Door Opener" experience in Apple Home without the complexity of Matter or external bridges.

## Why HomeSpan instead of Matter?
- **Smaller Code Size**: Significantly reduced firmware footprint compared to the Matter stack.
- **Native Experience**: Provides a native Apple Garage Door Opener accessory with full status feedback.
- **Improved Performance**: Faster response times and lighter CPU load on single-core chips like the ESP32-C6.
- **Built-in Web Log**: Includes a local StatusLog webpage for easy remote monitoring.

## Hardware Requirements
- **Microcontroller**: [QSZNTEC ESP32-C6 Mini](https://github.com/qszntec/ESP32-C6-Mini) (Single-core RISC-V)
- **Relay**: HFD4/3-S Relay Module (Pulsed dry contact)
- **Sensor**: Magnetic Reed Switch (Normally Closed)
- **Power**: LM2596 DC-DC Step Down Converter (for stable supply from gate power)
- **Antenna**: Waterproof Outdoor Antenna (for extended range)

## Pin Configuration (ESP32-C6 Mini)
| Component | GPIO Pin | Description |
|-----------|----------|-------------|
| **Relay** | GPIO 0 | D0 (Active Low Pulse) |
| **Reed Sensor** | GPIO 8 | D8 (LOW = Closed, HIGH = Open) |
| **Status LED** | GPIO 15 | Onboard SK6812 (Multicolor) |
| **Button** | GPIO 9 | Onboard BOOT Button (Control) |

## Features
- **Native HomeKit**: Setup directly in the Apple Home app via QR code or pairing code.
- **Stability Hardening**: 
  - **10Hz Logic Loop**: Reduced CPU load to prevent watchdog resets.
  - **Sensor Debouncing**: 250ms filter to prevent ghost triggers and NVS spam.
  - **Task Watchdog**: 30-second safety net for maximum reliability.
- **Multicolor LED Status**:
  - **RED**: WiFi Disconnected.
  - **YELLOW**: Gate is OPEN.
  - **Flashing PURPLE**: Relay active / Request processing.
  - **OFF**: Normal (Closed & Connected).
- **Web Status Log**: View recent activity at `http://homespan-gate.local/StatusLog`.

## Installation & Setup
1. Open `HomeSpan_Gate.ino` in the Arduino IDE.
2. Ensure you have the **HomeSpan** library installed (v2.1.0+).
3. Select **ESP32C6 Dev Module** as your board.
4. Upload the sketch and open the Serial Monitor (115200 baud).
5. Follow the HomeSpan pairing instructions to add the "Gate Controller" to your Apple Home.

## Directory Structure
- `HomeSpan_Gate/`: Main Arduino sketch folder.
- `LIBRARIES.md`: Documentation for required dependencies.
- `QUICK_START.md`: Step-by-step guide for first-time setup.

---
*Created with ❤️ for HomeKit enthusiasts.*
