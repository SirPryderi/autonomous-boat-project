#pragma once
#include <WiFi.h>

#ifndef WIFI_SSID
#error "WIFI_SSID not defined!"
#endif

static const char* ssid = WIFI_SSID;
static const char* pass = WIFI_PASS;

class WifiManager {
 public:
  void connectToWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);

    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection Failed! Rebooting...");
      delay(5000);
      ESP.restart();
    }

    Serial.printf("WIFI connected to %s\n", ssid);
    Serial.printf("IP address: %s\n", WiFi.localIP().toString());
  }
};