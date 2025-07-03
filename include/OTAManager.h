#pragma once
#include <ArduinoOTA.h>

#ifndef WIFI_SSID
#error "WIFI_SSID not defined!"
#endif

static const char* ssid = WIFI_SSID;
static const char* pass = WIFI_PASS;

class OtaManager {
 public:
  void begin(const char* hostname = nullptr) {
    WiFi.begin(ssid, pass);
    WiFi.waitForConnectResult();

    if (hostname)
      ArduinoOTA.setHostname(hostname);

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
  }

  void handle() {
    ArduinoOTA.handle();
  }
};
