#pragma once

#include <Display.h>

const int pwmChannel = 0;
const int freq = 50000;
const int resolution = 8;
const int maxDuty = (1 << resolution) - 1;

class MotorController {
 public:
  void begin() {
    digitalWrite(PIN_MOTOR_SLP, LOW);
    pinMode(PIN_MOTOR_IN1, OUTPUT);
    pinMode(PIN_MOTOR_IN2, OUTPUT);
    ledcSetup(pwmChannel, freq, resolution);
    ledcAttachPin(PIN_MOTOR_IN2, pwmChannel);
    ledcWrite(pwmChannel, 0);
  }

  void setMotor(int speedRange) {
    speedRange = constrain(speedRange, -100, 100);

    if (speedRange == 0) {
      Display::render("SLP", 4);
      digitalWrite(PIN_MOTOR_SLP, LOW);
      digitalWrite(PIN_MOTOR_IN1, LOW);
      digitalWrite(PIN_MOTOR_IN2, LOW);
      ledcWrite(pwmChannel, 0);
      Display::render("R - OFF", 5);
      return;
    }

    bool forward = speedRange > 0;
    int duty = map(abs(speedRange), 0, 100, 0, maxDuty);

    float baseVoltage = 5.0;
    float aproxVoltage = (baseVoltage * duty) / maxDuty;

    Display::render("", 4);
    String motorSummaryR = "R  " + String(speedRange) + "% " + String(duty) + "pwm ~" + String(aproxVoltage) + "V";
    Display::render(motorSummaryR.c_str(), 5);

    digitalWrite(PIN_MOTOR_SLP, HIGH);
    digitalWrite(PIN_MOTOR_IN1, forward ? LOW : HIGH);
    ledcWrite(pwmChannel, duty);
  }

  void handle() {
  }
};
