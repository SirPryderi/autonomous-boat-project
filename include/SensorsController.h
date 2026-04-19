#pragma once
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT22  // DHT 11

DHT_Unified dht(PIN_TEMP_SENSOR, DHTTYPE);

int lastCheckMillis = 0;
const int intervalMillis = 2000;

class SensorController {
 public:
  static void begin() {
    dht.begin();
  }

  static void handle() {
    int currentMillis = millis();

    if (currentMillis - lastCheckMillis < intervalMillis) {
      return;
    }

    lastCheckMillis = millis();

    sensors_event_t event;

    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.println(F("Error reading temperature!"));
    } else {
      Serial.print(F("Temperature: "));
      Serial.print(event.temperature);
      Serial.println(F("°C"));
    }

    float temperature = event.temperature;

    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println(F("Error reading humidity!"));
    } else {
      Serial.print(F("Humidity: "));
      Serial.print(event.relative_humidity);
      Serial.println(F("%"));
    }

    char buf[16];
    snprintf(buf, sizeof(buf), "%d \xF7", temperature);
    Display::render(buf, 1);
  }
};