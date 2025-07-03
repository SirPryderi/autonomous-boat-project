#include <Arduino.h>

#ifdef ESP32_BUILD
#include <BoatController.h>
BoatController boatController;
#endif

#ifdef ARDUINO_UNO_BUILD
#include <UnoController.h>
UnoController unoController;
#endif

void setup() {
  Serial.begin(SERIAL_SPEED);

#ifdef ESP32_BUILD
  boatController.begin();
#endif

#ifdef ARDUINO_UNO_BUILD
  unoController.begin();
#endif

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.println("[@] Setup complete");
}

void loop() {
#ifdef ESP32_BUILD
  boatController.handle();
#endif
#ifdef ARDUINO_UNO_BUILD
  unoController.handle();
#endif
}