#pragma once
#include <Arduino.h>

class UnoController {
 public:
  void begin() {
    Serial.println("[@] Hello from Arduino Uno!");
  }

  void handle() {
    digitalWrite(PIN_LED, HIGH);
    delay(1000);
    digitalWrite(PIN_LED, LOW);
    delay(1000);
  }
};
