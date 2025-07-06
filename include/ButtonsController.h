#pragma once
#include <Arduino.h>

#include <functional>

#define MAX_BUTTONS 10

class ButtonsController {
  struct Button {
    uint8_t pin;
    std::function<void()> callback;
    bool lastState = HIGH;
    bool currentState = HIGH;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;
    bool released = true;
  };

  Button buttons[MAX_BUTTONS];
  int count = 0;

 public:
  ButtonsController& onButtonPress(uint8_t pin, std::function<void()> callback) {
    if (count >= MAX_BUTTONS)
      return *this;
    buttons[count].pin = pin;
    buttons[count].callback = callback;
    pinMode(pin, INPUT_PULLUP);
    count++;
    return *this;
  }

  void handle() {
    unsigned long currentMillis = millis();
    for (int i = 0; i < count; ++i) {
      Button& b = buttons[i];
      b.currentState = digitalRead(b.pin);

      if (b.currentState != b.lastState) {
        b.lastDebounceTime = currentMillis;
      }

      if ((currentMillis - b.lastDebounceTime) > b.debounceDelay) {
        if (b.currentState == LOW && b.released) {
          b.callback();
          b.released = false;
        } else if (b.currentState == HIGH && !b.released) {
          b.released = true;
        }
      }

      b.lastState = b.currentState;
    }
  }
};

ButtonsController Buttons;
