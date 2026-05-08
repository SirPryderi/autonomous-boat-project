#pragma once
#include <Adafruit_INA219.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT22

Adafruit_INA219 ina219;
DHT_Unified dht(PIN_TEMP_SENSOR, DHTTYPE);

int lastCheckMillis = 0;
const int intervalMillis = 1000;

const char* $(const char* format, ...) {
  static char buf[20];
  va_list args;
  va_start(args, format);
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  return buf;
}

class SensorController {
 public:
  static void begin() {
    dht.begin();
    ina219.begin();
  }

  static void handle() {
    int currentMillis = millis();

    if (currentMillis - lastCheckMillis < intervalMillis) {
      return;
    }

    lastCheckMillis = millis();

    sensors_event_t event;

    dht.temperature().getEvent(&event);
    float temperature = event.temperature;
    dht.humidity().getEvent(&event);
    float humidity = event.relative_humidity;

    Display::render($("%.2f \xF7"
                      "C %.2f%%",
                      temperature,
                      humidity),
      1);

    float busVoltage = ina219.getBusVoltage_V();
    float shuntVoltage = ina219.getShuntVoltage_mV() / 1000;
    float current = ina219.getCurrent_mA();
    float trueVoltage = busVoltage + shuntVoltage;

    auto batteryLow_V = 4.80;
    auto batteryHigh_V = 5.00;

    auto batteryPercentage = (trueVoltage - batteryLow_V) / (float)(batteryHigh_V - batteryLow_V) * 100.0f;

    if (batteryPercentage > 100) {
      batteryPercentage = 100;
    }

    Display::render($("%.2fV %.2fmA %.0f%%", trueVoltage, current, batteryPercentage), 2);
  }
};