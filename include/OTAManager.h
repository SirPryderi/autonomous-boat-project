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
        Display::render("OTA start", 1);
      })
      .onEnd([]() {
        Serial.println("\n[@] OTA end");
        Display::render("OTA end", 1);
      })
      .onProgress([](unsigned int p, unsigned int t) {
        Serial.printf("[@] OTA: %u%%\r", (p * 100) / t);
        Display::render((String("OTA ") + String((p * 100) / t)).c_str(), 1);
      })
      .onError([](ota_error_t e) {
        Serial.printf("[!] OTA error[%u]\n", e);
        Display::render(("[!] OTA error: " + String(e)).c_str(), 1);
      });

    ArduinoOTA.begin();

    Serial.println("Ready for OTA");
  }

  void handle() {
    ArduinoOTA.handle();
  }
};
