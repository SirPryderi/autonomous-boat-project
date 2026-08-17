#pragma once
#include <ButtonsController.h>
#include <Display.h>
#include <IBusBM.h>
#include <MotorController.h>
#include <PCF8574.h>

IBusBM IBus;

void handleIBus(void* pvParameters) {
  while (1) {
    IBus.loop();
  }
}

class RemoteController {
 private:
  MotorController& motorController;
  long throttle = 0;
  long steering = 0;
  long turning = 0;

 public:
  RemoteController(MotorController& _motorController) : motorController(_motorController) {}

  void begin() {
    IBus.begin(Serial2, IBUSBM_NOTIMER);

    xTaskCreatePinnedToCore(
      handleIBus,
      "handleIBus",
      1000,
      NULL,
      0,
      NULL,
      0);
  }

  void handle() {
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

    if (chan1 >= 1000 || chan3 >= 1000) {
      int mappedThrottle = map(chan3, 1000, 2000, 0, 1000);
      int mappedSteering = map(chan4, 1000, 2000, -1000, 1000);
      int mappedTurning = map(chan1, 1000, 2000, -1000, 1000);

      // TODO: should move the no-op to the setMotors function, move. Set all state variable to the motor controller as well
      if (mappedThrottle != throttle || mappedSteering != steering || mappedTurning != turning) {
        throttle = mappedThrottle;
        steering = mappedSteering;
        turning = mappedTurning;
        motorController.setMotors(throttle, steering, turning);
      }
    }
  }
};