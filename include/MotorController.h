#pragma once
#include <Display.h>

#include <tuple>

// channels 1/2 are reserved for tones
const uint8_t PWM_CHANNEL_A_1 = 2;
const uint8_t PWM_CHANNEL_A_2 = 3;
const uint8_t PWM_CHANNEL_B_1 = 4;
const uint8_t PWM_CHANNEL_B_2 = 5;

const uint32_t freq = 20000;  // lower: better low duty cycles, higher than 20k: not audible whine
const uint8_t resolution = 10;
const uint32_t maxDutyRange = (1 << resolution) - 1;

const uint8_t minDutyPercent = 05;  // %
const uint8_t maxDutyPercent = 50;  // %

const uint32_t minDuty = maxDutyRange * minDutyPercent / 100u;
const uint32_t maxDuty = maxDutyRange * maxDutyPercent / 100u;

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

  void setMotors(int throttle, int steering = 0, int turning = 0) {
    throttle = constrain(throttle, -1000, 1000);
    steering = constrain(steering, -1000, 1000);
    turning = constrain(turning, -1000, 1000);

    if (throttle == 0 && turning == 0) {
      digitalWrite(PIN_MOTOR_STBY, LOW);
      setMotorInternal(0, PWM_CHANNEL_A_1, PWM_CHANNEL_A_2);
      setMotorInternal(0, PWM_CHANNEL_B_1, PWM_CHANNEL_B_2);
      Display::render("SLP R OFF  L OFF", 3);
      return;
    }

    digitalWrite(PIN_MOTOR_STBY, HIGH);

    int motor1Speed, motor2Speed;
    std::tie(motor1Speed, motor2Speed) =
      turning == 0
        ? getDifferentialEngineThrottles(throttle, steering)
        : turn(turning);

    setMotorInternal(motor1Speed, PWM_CHANNEL_A_1, PWM_CHANNEL_A_2);
    setMotorInternal(motor2Speed, PWM_CHANNEL_B_1, PWM_CHANNEL_B_2);

    char motorSummary[20];
    sprintf(motorSummary, "    R %03d%% L %03d%%", motor1Speed / 10, motor2Speed / 10);
    Display::render(motorSummary, 3);
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
      ledcWrite(pwmChannel1, maxDutyRange);
      ledcWrite(pwmChannel2, maxDutyRange - duty);
    } else {
      ledcWrite(pwmChannel2, maxDutyRange);
      ledcWrite(pwmChannel1, maxDutyRange - duty);
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

  std::tuple<int, int> turn(int turning) {
    int motor1Speed = -turning;
    int motor2Speed = turning;

    return {motor1Speed, motor2Speed};
  }
};
