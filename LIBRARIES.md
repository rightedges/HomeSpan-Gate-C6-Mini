# Library Requirements - HomeSpan Gate Controller

This project relies on the **HomeSpan** library, which provides a high-level API for creating native HomeKit accessories on the ESP32.

## Required Libraries

### 1. HomeSpan
- **Version**: 2.1.0 or later (Tested with 2.1.7)
- **Author**: Gregg Berg (HomeSpan)
- **Purpose**: Provides the HomeKit protocol stack and accessory management.
- **Install**: In Arduino IDE, go to **Sketch → Include Library → Manage Libraries**, search for **"HomeSpan"**, and install.

## Board Support Package

### ESP32 by Espressif Systems
- **Version**: 3.0.0 or later (Tested with 3.3.3)
- **Purpose**: Provides the underlying ESP-IDF and hardware drivers for the ESP32-C6.
- **Install**: See [QUICK_START.md](QUICK_START.md) for detailed instructions.

## Why No Other Libraries?
HomeSpan is a comprehensive "all-in-one" solution for HomeKit on ESP32. It handles:
- ✅ **HomeKit Accessory Protocol (HAP)**
- ✅ **mDNS / Bonjour Service Discovery**
- ✅ **WiFi Provisioning (via CLI or Access Point)**
- ✅ **NVS Storage (for pairing and settings)**
- ✅ **OTA Firmware Updates**
- ✅ **Web Log Server**

By using HomeSpan, we avoid the need for external Matter, HAP-Client, or multiple individual sensor/WiFi libraries, resulting in a cleaner and more stable codebase for the single-core ESP32-C6.
