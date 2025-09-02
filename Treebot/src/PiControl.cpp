#include "PiControl.h"

void PiControl::begin(uint8_t relayPin, uint8_t shutdownReqPin, uint8_t ackPin) {
  relayPin_ = relayPin; reqPin_ = shutdownReqPin; ackPin_ = ackPin;
  pinMode(relayPin_, OUTPUT);
  pinMode(reqPin_, OUTPUT);
  pinMode(ackPin_, INPUT_PULLUP); // Pi pulls LOW to ack
  // Relay modules are usually LOW=ON, HIGH=OFF:
  digitalWrite(relayPin_, HIGH);   // OFF by default
  digitalWrite(reqPin_, LOW);      // no shutdown requested
}

void PiControl::powerOn() {
  digitalWrite(relayPin_, LOW);  // energize relay → power ON
  powered = true; req = false;
}

void PiControl::requestShutdown() {
  if (powered && !req) {
    digitalWrite(reqPin_, HIGH); // signal Pi to shutdown
    req = true;
  }
}

bool PiControl::ackReceived() const {
  return digitalRead(ackPin_) == LOW; // LOW = Pi says "safe to cut (soon)"
}

void PiControl::powerOff() {
  digitalWrite(relayPin_, HIGH); // de-energize relay → power OFF
  powered = false; req = false;
}
