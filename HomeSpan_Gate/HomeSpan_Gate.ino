#include "HomeSpan.h"

// --- Configuration for ESP32-C6 Mini ---
const int CONTROL_PIN = 9;        // Boot Button
const int STATUS_LED_PIN = 15;    // Onboard SK6812 RGB LED
const int REED_SENSOR_PIN = 8;    // Reed sensor (D8)
const int RELAY_PIN = 0;          // Relay (D0)
const int RELAY_PULSE_MS = 1000;  // 1-second relay pulse

// --- HomeSpan Garage Door Opener Service ---
struct GateController : Service::GarageDoorOpener {
  SpanCharacteristic *current;
  SpanCharacteristic *target;
  SpanCharacteristic *obstruction;
  unsigned long pulseStartTime = 0;  // 0 means no pulse active
  unsigned long lastLoopMillis = 0;  // Throttle timer

  // Stability Hardening Variables
  int lastRawSensor = -1;
  unsigned long lastRawTime = 0;
  int debouncedSensor = -1;
  bool targetUpdatePending = false;
  int pendingTargetState = -1;
  unsigned long lastCommandTime = 0; // Prevent duplicate triggers during delayed starts

  GateController()
    : Service::GarageDoorOpener() {

    current = new Characteristic::CurrentDoorState(1);  // 1 = Closed
    target = new Characteristic::TargetDoorState(1);    // 1 = Closed
    obstruction = new Characteristic::ObstructionDetected(false);

    // Initialize Relay to OFF (Active-High: LOW = OFF, HIGH = ON)
    // Using ESP-IDF functions guarantees NO microsecond glitches during setup
    gpio_set_level((gpio_num_t)RELAY_PIN, 0);
    gpio_set_direction((gpio_num_t)RELAY_PIN, GPIO_MODE_OUTPUT);
    pinMode(REED_SENSOR_PIN, INPUT_PULLUP);

    WEBLOG("Gate Controller: Initialized and ready.");

    // Initial sync with physical sensor
    syncSensor();
  }

  boolean update() override {
    if (target->updated()) {
      int newVal = target->getNewVal();
      int currentState = current->getVal();

      // Only trigger if the target state is different from the current physical state
      if (newVal != currentState) {
        
        // Prevent redundant pulses if the SAME command is sent while gate is delayed/preparing to move
        if (newVal == pendingTargetState && (millis() - lastCommandTime < 20000)) {
          WEBLOG("Request ignored: Gate is already preparing to %s", newVal == 0 ? "OPEN" : "CLOSED");
        } else {
          pendingTargetState = newVal;
          targetUpdatePending = true;  // Signal for loop() to process
          lastCommandTime = millis();
          triggerRelay();
        }
      } else {
        WEBLOG("Request ignored: Gate is already %s", newVal == 0 ? "OPEN" : "CLOSED");
      }
    }
    return true;
  }

  void loop() override {
    // 1. Throttle Logic to 100ms (10Hz) to prevent CPU starvation on C6 Mini
    if (millis() - lastLoopMillis < 100) return;
    lastLoopMillis = millis();

    // 2. Debounce and Sync Sensor
    int raw = digitalRead(REED_SENSOR_PIN);
    if (raw != lastRawSensor) {
      lastRawTime = millis();
      lastRawSensor = raw;
    }

    if (debouncedSensor != raw && (millis() - lastRawTime > 250)) {
      debouncedSensor = raw;
      syncSensor();  // Now only called if stable for 250ms
    }

    // 3. Process Pending HomeKit Commands (to keep update() fast)
    if (targetUpdatePending) {
      targetUpdatePending = false;
      WEBLOG("HomeKit request: Set Target to %s", pendingTargetState == 0 ? "OPEN" : "CLOSED");
    }

    // 4. Handle Pulse State
    if (pulseStartTime > 0) {
      unsigned long elapsed = millis() - pulseStartTime;
      if (elapsed < RELAY_PULSE_MS) {
        // Flash logic DISABLED for native LED test
      } else {
        // Pulse Complete
        gpio_set_level((gpio_num_t)RELAY_PIN, 0);  // Relay OFF
        pulseStartTime = 0;
        WEBLOG("Relay pulse complete.");
      }
    } else {
      // 5. Multicolor status logic DISABLED for native LED test
    }
  }

  void updateStatusLed() {
    // Manual logic DISABLED for native LED test
  }

  void triggerRelay() {
    WEBLOG("Pulsing relay...");
    gpio_set_level((gpio_num_t)RELAY_PIN, 1);  // Relay ON (Active-High)
    pulseStartTime = millis();
    if (pulseStartTime == 0) pulseStartTime = 1;  // Prevent 0
  }

  void syncSensor() {
    // Reed Sensor: LOW = Closed (1), HIGH = Open (0)
    int physicalState = (debouncedSensor == LOW) ? 1 : 0;

    if (current->getVal() != physicalState) {
      WEBLOG("Sensor: Gate %s", physicalState == 1 ? "CLOSED" : "OPEN");

      current->setVal(physicalState);

      // Also sync target state to avoid "No Response" or "Opening..." stuck UI
      if (target->getVal() != physicalState) {
        target->setVal(physicalState);
      }
    }
  }
};

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n\n--- HomeSpan Gate Controller (ESP32C6 mini) ---");

  // HomeSpan Setup
  homeSpan.setControlPin(CONTROL_PIN);
  homeSpan.setStatusPixel(STATUS_LED_PIN);  // Use HomeSpan's native NeoPixel status LED
  homeSpan.enableWatchdog(30);              // Software safety net (30s)
  homeSpan.setLogLevel(1);                  // Enable minimal logs for serial monitoring
  // homeSpan.enableWebLog(10, "pool.ntp.org", "UTC", "StatusLog"); // DISABLED FOR ISOLATION TEST
  homeSpan.begin(Category::GarageDoorOpeners, "Gate Controller");

  // Define Accessory
  new SpanAccessory();
  new Service::AccessoryInformation();
  new Characteristic::Identify();
  new Characteristic::Manufacturer("HomeSpan");
  new Characteristic::SerialNumber("GATE-01");
  new Characteristic::Model("ESP32C6-mini");
  new Characteristic::FirmwareRevision("1.0.0");

  // Add the Gate Controller Service
  new GateController();
}

void loop() {
  homeSpan.poll();
  delay(1);  // Yield to system tasks on single-core C6
}
