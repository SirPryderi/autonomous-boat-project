
#pragma once
#include <ArduinoOTA.h>
#include <ButtonsController.h>
#include <Display.h>
#include <IBusBM.h>
#include <MotorController.h>
#include <OTAManager.h>
#include <WifiManager.h>

OtaManager otaManager;
WifiManager wifiManager;
MotorController motorController;
IBusBM IBus;

long throttle = 0;
long steering = 0;
long turning = 0;

void handleIBus(void* pvParameters) {
  while (1) {
    IBus.loop();
  }
}

class BoatController {
 public:
  void begin() {
    Serial.println("[@] Hello from ESP32!");
    Display::begin();
    motorController.begin();
    wifiManager.connectToWifi();

    otaManager.begin();

    Buttons
      .onButtonPress(GPIO_NUM_32, []() {
        motorController.setMotors(throttle -= 20);
      })
      .onButtonPress(GPIO_NUM_33, []() {
        motorController.setMotors(throttle += 20);
      });

    IBus.begin(Serial2, IBUSBM_NOTIMER);

    xTaskCreatePinnedToCore(
      handleIBus,    // Function to implement the task
      "handleIBus",  // Name of the task
      1000,          // Stack size in bytes
      NULL,          // Task input parameter
      0,             // Priority of the task
      NULL,          // Task handle.
      0              // Core where the task should run
    );
  }

  void handle() {
    otaManager.handle();
    motorController.handle();
    Buttons.handle();

    int chan1 = IBus.readChannel(0);
    int chan2 = IBus.readChannel(1);
    int chan3 = IBus.readChannel(2);  // Left stick - vertical
    int chan4 = IBus.readChannel(3);  // Left stick - horizontal
    int chan5 = IBus.readChannel(4);
    int chan6 = IBus.readChannel(5);

    String valLine1 = "1:" + String(chan1 / 1000.0) + " 2:" + String(chan2 / 1000.0) + " 3:" + String(chan3 / 1000.0);
    String valLine2 = "4:" + String(chan4 / 1000.0) + " 5:" + String(chan5 / 1000.0) + " 6:" + String(chan6 / 1000.0);

    Display::render(String(valLine1).c_str(), 5);
    Display::render(String(valLine2).c_str(), 6);

    if (chan1 >= 1000) {
      int mappedThrottle = map(chan2, 1000, 2000, -1000, 1000);
      int mappedSteering = map(chan1, 1000, 2000, -1000, 1000);
      int mappedTurning = map(chan4, 1000, 2000, -1000, 1000);

      if (mappedThrottle != throttle || mappedSteering != steering || mappedTurning != turning) {
        throttle = mappedThrottle;
        steering = mappedSteering;
        turning = mappedTurning;
        motorController.setMotors(throttle, steering, turning);
      }
    }
  }
};
