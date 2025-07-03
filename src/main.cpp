#include <Arduino.h>

void setup() {
  Serial.begin(SERIAL_SPEED);

#ifdef ESP32_BUILD
  Serial.println("Hello from ESP32!");
#endif

#ifdef ARDUINO_UNO_BUILD
  Serial.println("Hello from Arduino Uno!");
#endif

  pinMode(LED_PIN, OUTPUT);

  Serial.println("Setup complete");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}