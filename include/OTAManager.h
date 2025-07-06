#pragma once
#include <ArduinoOTA.h>

static const char* hostname = OTA_HOSTNAME;
static const char* password = OTA_PASSWORD;

class OtaManager {
 public:
  void begin() {
    ArduinoOTA.setHostname(hostname);
    ArduinoOTA.setPassword(password);

    ArduinoOTA
      .onStart([]() {
        Serial.println("[@] OTA start");
      })
      .onEnd([]() {
        Serial.println("\n[@] OTA end");
      })
      .onProgress([](unsigned int p, unsigned int t) {
        Serial.printf("[@] OTA: %u%%\r", (p * 100) / t);
      })
      .onError([](ota_error_t e) {
        Serial.printf("[!] OTA error[%u]\n", e);
      });

    ArduinoOTA.begin();

    Serial.println("Ready for OTA");
  }

  void handle() {
    ArduinoOTA.handle();
  }
};
