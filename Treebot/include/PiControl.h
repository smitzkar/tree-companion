#pragma once // simpler alternative to ifndef to guard against multiple inclusions 
#include <Arduino.h>

class PiControl {
    
public:
  void begin(uint8_t relayPin, uint8_t shutdownReqPin, uint8_t ackPin);
  void powerOn();                    // closes relay (power to Pi)
  void requestShutdown();            // drives shutdown_req HIGH
  bool ackReceived() const;          // reads Pi ack pin (LOW = ack)
  void powerOff();                   // opens relay (cuts power)
  bool isPowered() const { return powered; }
  bool shutdownRequested() const { return req; }

private:
  uint8_t relayPin_, reqPin_, ackPin_;
  bool powered = false, req = false;
};
