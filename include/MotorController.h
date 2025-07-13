#pragma once

#include <Display.h>

#include <tuple>

const uint8_t PWM_CHANNEL_A_1 = 0;
const uint8_t PWM_CHANNEL_A_2 = 1;
const uint8_t PWM_CHANNEL_B_1 = 2;
const uint8_t PWM_CHANNEL_B_2 = 4;

const int freq = 50000;
const int resolution = 8;
const int maxDutyRange = (1 << resolution) - 1;

const double minDutyPercent = 0.10;
const double maxDutyPercent = 0.15;

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

    setupChannel(PWM_CHANNEL_A_1, PIN_MOTOR_A_IN1);
    setupChannel(PWM_CHANNEL_A_2, PIN_MOTOR_A_IN2);

    setupChannel(PWM_CHANNEL_B_1, PIN_MOTOR_B_IN1);
    setupChannel(PWM_CHANNEL_B_2, PIN_MOTOR_B_IN2);

    setMotors(0, 0);
  }

  void setMotors(int throttle, int steering = 0) {
    throttle = constrain(throttle, -1000, 1000);
    steering = constrain(steering, -1000, 1000);

    if (throttle == 0) {
      Display::render("SLP", 2);
      digitalWrite(PIN_MOTOR_STBY, LOW);
      setMotorInternal(0, PWM_CHANNEL_A_1, PWM_CHANNEL_A_2);
      setMotorInternal(0, PWM_CHANNEL_B_1, PWM_CHANNEL_B_2);
      Display::render("R - OFF", 3);
      Display::render("L - OFF", 4);
      return;
    }

    digitalWrite(PIN_MOTOR_STBY, HIGH);

    int motor1Speed, motor2Speed;
    std::tie(motor1Speed, motor2Speed) = getDifferentialEngineThrottles(throttle, steering);

    setMotorInternal(motor1Speed, PWM_CHANNEL_A_1, PWM_CHANNEL_A_2);
    setMotorInternal(motor2Speed, PWM_CHANNEL_B_1, PWM_CHANNEL_B_2);

    Display::render("", 2);
    String motorSummaryR = "R  " + String(motor1Speed / 10) + " %";
    String motorSummaryL = "L  " + String(motor2Speed / 10) + " %";
    Display::render(motorSummaryR.c_str(), 3);
    Display::render(motorSummaryL.c_str(), 4);
  }

  void handle() {
  }

 private:
  void setMotorInternal(int throttle, int pwmChannel1, int pwmChannel2) {
    throttle = constrain(throttle, -1000.0, 1000.0);

    if (throttle == 0) {
      ledcWrite(pwmChannel1, 0);
      ledcWrite(pwmChannel2, 0);
      return;
    }

    bool forward = throttle > 0;
    uint32_t duty = map(abs(throttle), 0, 1000, minDuty, maxDuty);

    if (forward) {
      ledcWrite(pwmChannel1, duty);
      ledcWrite(pwmChannel2, 0);
    } else {
      ledcWrite(pwmChannel2, duty);
      ledcWrite(pwmChannel1, 0);
    }
  }

  void setupChannel(uint8_t channel, uint8_t pin) {
    ledcSetup(channel, freq, resolution);
    ledcAttachPin(pin, channel);
    ledcWrite(channel, 0);
  }

  std::tuple<int, int> getDifferentialEngineThrottles(int throttle, int steering) {
    // Differential drive truth table:
    // T: 0     S: 0      = L: 0     R: 0
    // T: 1000  S: 0      = L: 1000  R: 1000
    // T: 1000  S: 1000   = L: 1000  R: 0
    // T: 1000  S: -1000  = L: 0     R: 1000
    // T: 500   S: 1000   = L: 500   R: 0
    // T: 1000  S: 500    = L: 1000  R: 500

    int motor1Speed = 0;
    int motor2Speed = 0;

    if (steering > 0) {
      // Forward right turn
      motor1Speed = map(steering, 0, 1000, throttle, 0);
      motor2Speed = throttle;
    } else if (steering < 0) {
      // Forward left turn
      motor1Speed = throttle;
      motor2Speed = map(steering, 0, -1000, throttle, 0);
    } else {
      // Forward straight
      motor1Speed = throttle;
      motor2Speed = throttle;
    }

    return {motor1Speed, motor2Speed};
  }
};
