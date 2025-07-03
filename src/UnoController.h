#pragma once
#include <Arduino.h>

class UnoController {
 public:
  void begin() {
    Serial.println("[@] Hello from Arduino Uno!");
  }

  void handle() {
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
  }
};
