#pragma once
#include <PCF8574.h>

volatile bool keyPressed = false;

void ICACHE_RAM_ATTR keyPressedOnPCF8574() {
  keyPressed = true;
}

PCF8574 expander0x20(0x20, PIN_SDA, PIN_SCL, 34, keyPressedOnPCF8574);

PCF8574::DigitalInput expanderStatus;

class ExpanderController {
 public:
  static void begin() {
    expander0x20.pinMode(P0, INPUT_PULLUP);
    expander0x20.pinMode(P1, INPUT_PULLUP);
    expander0x20.pinMode(P2, INPUT_PULLUP);
    expander0x20.pinMode(P3, INPUT_PULLUP);
    expander0x20.pinMode(P4, INPUT_PULLUP);
    expander0x20.pinMode(P5, INPUT_PULLUP);
    expander0x20.pinMode(P6, INPUT_PULLUP);
    expander0x20.pinMode(P7, INPUT_PULLUP);

    if (!expander0x20.begin()) {
      Serial.println("[!] Couldn't find I/O expansion 0x20");
    } else {
      Serial.println("[@] Connected to I/O expansion 0x20");
    }
  }

  static void handle() {
    if (keyPressed) {
      keyPressed = false;
      expanderStatus = expander0x20.digitalReadAll();
      if (expanderStatus.p7 == LOW) {
        tone(PIN_BUZZER, 800, 200);
      } else {
        tone(PIN_BUZZER, 500, 200);
      }
    }
  }
};