#include <Arduino.h>
#include "PiControl.h"
// #include "WifiManager.h"
// #include "Sensors.h"
// #include "Logger.h"

PiControl pi;
const uint8_t PIN_RELAY = 1;           // S on relay
const uint8_t PIN_SHUTDOWN_REQ = 2;   // ESP32 → Pi GPIO17
const uint8_t PIN_PI_ACK = 3;         // Pi GPIO27 → ESP32

void setup() {
  Serial.begin(115200);
  pi.begin(PIN_RELAY, PIN_SHUTDOWN_REQ, PIN_PI_ACK);

  // Power the Pi for demo
  pi.powerOn();
  Serial.println("Pi power ON (relay closed).");
}

void loop() {
  static bool asked = false;
  static uint32_t t_ack = 0;

  // After 20 s, ask Pi to shutdown
  if (!asked && millis() > 20000) {
    Serial.println("Requesting shutdown...");
    pi.requestShutdown();
    asked = true;
  }

  // When ack arrives, wait a conservative delay, then cut power
  if (asked && pi.ackReceived()) {
    if (t_ack == 0) {
      t_ack = millis();
      Serial.println("Ack received. Waiting 30 s before power cut...");
    } else if (millis() - t_ack > 30000) {
      pi.powerOff();
      Serial.println("Power OFF (relay open).");
      while (1) delay(1000);
    }
  }
  delay(200);
}
