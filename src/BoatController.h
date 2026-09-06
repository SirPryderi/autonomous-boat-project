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
ExpanderController expanderController;
ButtonsController buttons;

bool stealthMode = false;

class BoatController {
 public:
  void begin() {
    Serial.println("[@] Hello from BoatController!");
    Wire.begin(PIN_SDA, PIN_SCL);
    Display::begin();
    expanderController.begin();
    motorController.begin();
    remoteController.begin();
    sensorController.begin();

    stealthMode = expanderController.status.p7 == LOW;

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
    expanderController.handle();
    sensorController.handle();
    buttons.handle(&expanderController);
    Display::handle();
  }
};
