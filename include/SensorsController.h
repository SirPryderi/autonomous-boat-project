#pragma once
#include <Adafruit_INA219.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT22
#define BATTERY_COUNT 8

Adafruit_INA219 ina219;
DHT_Unified dht(PIN_TEMP_SENSOR, DHTTYPE);

bool ina219Ready = false;

const unsigned int intervalMillis = 1000;
unsigned long lastCheckMillis = -intervalMillis;

const char* $(const char* format, ...) {
  static char buf[20];
  va_list args;
  va_start(args, format);
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  return buf;
}

class SensorController {
 private:
  static const uint8_t temperatureLine = 2;
  static const uint8_t batteryLine = 4;

 public:
  static void begin() {
    dht.begin();

    scanI2C();

    ina219Ready = ina219.begin();
    if (!ina219Ready) {
      Serial.println("[!] INA219 I2C handshake failed");
    }
  }

  static void scanI2C() {
    Serial.println("[@] Scanning I2C bus...");

    int found = 0;
    for (uint8_t address = 1; address < 127; address++) {
      Wire.beginTransmission(address);
      if (Wire.endTransmission() == 0) {
        Serial.printf("[@]   device at 0x%02X\n", address);
        found++;
      }
    }

    if (found == 0) {
      Serial.println("[!]   no I2C devices found");
    }
  }

  static void handle() {
    auto currentMillis = millis();

    if (currentMillis - lastCheckMillis < intervalMillis) {
      return;
    }

    readTemperature();
    readVoltageCurrent();
  }

 private:
  static void readTemperature() {
    lastCheckMillis = millis();

    sensors_event_t event;

    dht.temperature().getEvent(&event);
    float temperature = event.temperature;
    dht.humidity().getEvent(&event);
    float humidity = event.relative_humidity;

    auto line = $(
      "%.2f \xF7"
      "C %.2f%%",
      temperature,
      humidity);
    Display::render(line, temperatureLine);
  }

  static void readVoltageCurrent() {
    ina219.powerSave(false);
    float busVoltage = ina219.getBusVoltage_V();
    float shuntVoltage = ina219.getShuntVoltage_mV() / 1000;
    float current = ina219.getCurrent_mA();
    float trueVoltage = busVoltage + shuntVoltage;

    if (!ina219Ready || !ina219.success()) {
      Serial.println("[!] INA219 read failed - retrying init");
      ina219Ready = ina219.begin();
      Display::render("BATT ERR", batteryLine);
      return;
    }

    ina219.powerSave(true);

    float cellAverage = trueVoltage / BATTERY_COUNT;

    float batteryLow_V = 1.00;
    float batteryWarning_V = 1.15;
    float batteryHigh_V = 1.30;

    auto batteryPercentage = (cellAverage - batteryLow_V) / (float)(batteryHigh_V - batteryLow_V) * 100.0f;

    if (batteryPercentage > 100) {
      batteryPercentage = 100;
    }

    if (batteryPercentage < 0) {
      batteryPercentage = 0;
    }

    Display::render($("%.2f%% %.2fV (%.2fV)", batteryPercentage, trueVoltage, cellAverage), batteryLine);
  }
};