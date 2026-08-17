#pragma once
#include <Display.h>
#include <ExpanderController.h>
#include <MotorController.h>
#include <OTAManager.h>
#include <RemoteController.h>
#include <SensorsController.h>
#include <WifiManager.h>
#include <Wire.h>

OtaManager otaManager;
WifiManager wifiManager;
MotorController motorController;
SensorController sensorController;
RemoteController remoteController(motorController);

bool stealthMode = false;

class BoatController {
 public:
  void begin() {
    Serial.println("[@] Hello from BoatController!");
    Wire.begin(PIN_SDA, PIN_SCL);
    Display::begin();
    ExpanderController::begin();
    motorController.begin();
    remoteController.begin();
    sensorController.begin();

    pinMode(PIN_STEALTH_SWITCH, INPUT_PULLUP);
    stealthMode = digitalRead(PIN_STEALTH_SWITCH) == HIGH;

    if (!stealthMode) {
      wifiManager.connectToWifi();
      otaManager.begin();
    } else {
      Serial.println("[@] No network mode");
      Display::render("No network", 0, true);
    }

    Serial.println("[@] Boot completed");
  }

  void handle() {
    if (!stealthMode) {
      otaManager.handle();
    }

    remoteController.handle();
    motorController.handle();
    ExpanderController::handle();
    sensorController.handle();
    Buttons.handle();
    Display::handle();
  }
};
