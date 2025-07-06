
#pragma once
#include <ArduinoOTA.h>
#include <OTAManager.h>
#include <WifiManager.h>

OtaManager otaManager;

WifiManager wifiManager;

class BoatController {
 public:
  void begin() {
    Serial.println("[@] Hello from ESP32!");
    wifiManager.connectToWifi();
    otaManager.begin();
  }

  void handle() {
    otaManager.handle();
  }
};
