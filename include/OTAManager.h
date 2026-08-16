#pragma once
#include <ArduinoOTA.h>

static const char* hostname = OTA_HOSTNAME;
static const char* password = OTA_PASSWORD;

class OtaManager {
 private:
  static const uint8_t displayLine = 2;

 public:
  void begin() {
    ArduinoOTA.setHostname(hostname);
    ArduinoOTA.setPassword(password);

    ArduinoOTA
      .onStart([]() {
        Serial.println("[@] OTA start");
        Display::render("OTA starting...", displayLine, true);
      })
      .onEnd([]() {
        Serial.println("\n[@] OTA end");
        Display::render("OTA completed!", displayLine, true);
      })
      .onProgress([](unsigned int p, unsigned int t) {
        Serial.printf("[@] OTA: %u%%\r", (p * 100) / t);
        Display::render(progressBar(p, t).c_str(), displayLine, true);
      })
      .onError([](ota_error_t e) {
        Serial.printf("[!] OTA error[%u]\n", e);
        Display::render(("[!] OTA error: " + String(e)).c_str(), displayLine, true);
        Serial.printf("[!] OTA error[%u]: ", e);
        if (e == OTA_AUTH_ERROR)
          Display::render("OTA Auth Failed", displayLine, true);
        else if (e == OTA_BEGIN_ERROR)
          Display::render("OTA Begin Failed", displayLine, true);
        else if (e == OTA_CONNECT_ERROR)
          Display::render("OTA Connect Failed", displayLine, true);
        else if (e == OTA_RECEIVE_ERROR)
          Display::render("OTA Receive Failed", displayLine, true);
        else if (e == OTA_END_ERROR)
          Display::render("OTA End Failed", displayLine, true);
      });

    ArduinoOTA.begin();

    Serial.printf("[@] OTA ready. Hostname: %s\n", ArduinoOTA.getHostname());
  }

  void handle() {
    ArduinoOTA.handle();
  }

 private:
  static String progressBar(unsigned int p, unsigned int t) {
    const uint8_t barWidth = 10;
    uint8_t filled = (p * barWidth) / t;

    String bar = "OTA [";
    for (uint8_t i = 0; i < barWidth; i++) {
      bar += (i < filled) ? "=" : " ";
    }
    bar += "] ";
    bar += String((p * 100) / t);
    bar += "%";
    return bar;
  }
};