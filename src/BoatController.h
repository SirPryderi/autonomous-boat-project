
#pragma once
#include <ArduinoOTA.h>
#include <OTAManager.h>

OtaManager otaManager;

class BoatController {
 public:
  void begin() {
    Serial.println("[@] Hello from ESP32!");
    otaManager.begin("esp32-boat");
  }

  void handle() {
    otaManager.handle();
  }
};
