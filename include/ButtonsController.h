#pragma once
#include <Arduino.h>

#include <functional>

#define MAX_BUTTONS 4

class ButtonsController {
  struct Button {
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
    buttons[count].callback = callback;
    count++;
    return *this;
  }

  void handle(ExpanderController* controller) {
    readButton(controller->status.p0, &buttons[0]);
    readButton(controller->status.p1, &buttons[1]);
    readButton(controller->status.p2, &buttons[2]);
    readButton(controller->status.p3, &buttons[3]);
  }

 private:
  void readButton(uint8_t value, Button* b) {
    unsigned long currentMillis = millis();

    b->currentState = value;

    if (b->currentState != b->lastState) {
      b->lastDebounceTime = currentMillis;
    }

    if ((currentMillis - b->lastDebounceTime) > b->debounceDelay) {
      if (b->currentState == LOW && b->released) {
        if (b->callback != nullptr)
          b->callback();
        b->released = false;
      } else if (b->currentState == HIGH && !b->released) {
        b->released = true;
      }
    }

    b->lastState = b->currentState;
  }
};
