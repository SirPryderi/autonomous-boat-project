#pragma once
#include <PCF8574.h>
#include <SensorsController.h>

volatile bool keyPressed = false;

void ICACHE_RAM_ATTR keyPressedOnPCF8574() {
  keyPressed = true;
}

PCF8574 expander0x20(0x20, PIN_SDA, PIN_SCL, 34, keyPressedOnPCF8574);

class ExpanderController {
 public:
  PCF8574::DigitalInput status;

 public:
  void begin() {
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
      return;
    }

    Serial.println("[@] Connected to I/O expansion 0x20");
    read();
  }

  void read() {
    keyPressed = false;
    status = expander0x20.digitalReadAll();
  }

  void handle() {
    if (keyPressed) {
      read();

      // Serial.println($("1:%d 2:%d 3:%d 4:%d",
      //   status.p0,
      //   status.p1,
      //   status.p2,
      //   status.p3));

      // Serial.println($("5:%d 6:%d 7:%d 8:%d",
      //   status.p4,
      //   status.p5,
      //   status.p6,
      //   status.p7));
    }
  }
};