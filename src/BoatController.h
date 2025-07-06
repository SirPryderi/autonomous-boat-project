
#pragma once
#include <ArduinoOTA.h>
#include <ButtonsController.h>
#include <Display.h>
#include <MotorController.h>
#include <OTAManager.h>
#include <WifiManager.h>

OtaManager otaManager;
WifiManager wifiManager;
MotorController motorController;

int speed = 0;

class BoatController {
 public:
  void begin() {
    Serial.println("[@] Hello from ESP32!");
    Display::begin();
    wifiManager.connectToWifi();
    otaManager.begin();
    motorController.begin();

    Buttons
      .onButtonPress(32, []() {
        speed -= 2;
        motorController.setMotor(speed);
      })
      .onButtonPress(33, []() {
        speed += 2;
        motorController.setMotor(speed);
      });
  }

  void handle() {
    otaManager.handle();
    motorController.handle();
    Buttons.handle();
  }
};
