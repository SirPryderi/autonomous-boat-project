#pragma once

#include <Display.h>

const int pwmChannel1 = 0;
const int pwmChannel2 = 1;

const int freq = 50000;
const int resolution = 8;
const int maxDutyRange = (1 << resolution) - 1;

const double minDutyPercent = 0.10;
const double maxDutyPercent = 0.75;

const uint32_t minDuty = static_cast<uint32_t>(maxDutyRange * minDutyPercent);
const uint32_t maxDuty = static_cast<uint32_t>(maxDutyRange * maxDutyPercent);

class MotorController {
 public:
  void begin() {
    pinMode(PIN_MOTOR_STBY, OUTPUT);
    pinMode(PIN_MOTOR_A_IN1, OUTPUT);
    pinMode(PIN_MOTOR_A_IN2, OUTPUT);
    pinMode(PIN_MOTOR_B_IN1, OUTPUT);
    pinMode(PIN_MOTOR_B_IN2, OUTPUT);

    digitalWrite(PIN_MOTOR_STBY, LOW);

    ledcSetup(pwmChannel1, freq, resolution);
    ledcAttachPin(PIN_MOTOR_A_IN2, pwmChannel1);
    ledcWrite(pwmChannel1, 0);

    ledcSetup(pwmChannel2, freq, resolution);
    ledcAttachPin(PIN_MOTOR_B_IN2, pwmChannel2);
    ledcWrite(pwmChannel2, 0);
  }

  void setMotors(long throttle, long balance = 0) {
    throttle = constrain(throttle, -1000, 1000);
    balance = constrain(balance, -1000, 1000);

    if (throttle == 0) {
      Display::render("SLP", 2);
      digitalWrite(PIN_MOTOR_STBY, LOW);
      setMotorInternal(0, PIN_MOTOR_A_IN1, PIN_MOTOR_A_IN2, pwmChannel1);
      setMotorInternal(0, PIN_MOTOR_B_IN1, PIN_MOTOR_B_IN2, pwmChannel2);
      Display::render("R - OFF", 3);
      Display::render("L - OFF", 4);
      return;
    }

    digitalWrite(PIN_MOTOR_STBY, HIGH);

    double motor1Speed = (double)throttle * (1.0 + balance / 1000.0);
    double motor2Speed = (double)throttle * (1.0 - balance / 1000.0);

    setMotorInternal(motor1Speed, PIN_MOTOR_A_IN1, PIN_MOTOR_A_IN2, pwmChannel1);
    setMotorInternal(motor2Speed, PIN_MOTOR_B_IN1, PIN_MOTOR_B_IN2, pwmChannel2);

    Display::render("", 2);
    String motorSummaryR = "R  " + String(motor1Speed / 10) + " %";
    String motorSummaryL = "L  " + String(motor2Speed / 10) + " %";
    Display::render(motorSummaryR.c_str(), 3);
    Display::render(motorSummaryL.c_str(), 4);
  }

  void handle() {
  }

 private:
  void setMotorInternal(double throttle, int pinIn1, int pinIn2, int pwmChannel) {
    throttle = constrain(throttle, -1000.0, 1000.0);

    if (throttle == 0) {
      digitalWrite(pinIn1, LOW);
      digitalWrite(pinIn2, LOW);
      ledcWrite(pwmChannel, 0);
      return;
    }

    bool forward = throttle > 0;
    uint32_t duty = map(abs(throttle), 0, 1000, minDuty, maxDuty);

    digitalWrite(pinIn1, forward ? LOW : HIGH);
    ledcWrite(pwmChannel, duty);
  }
};
