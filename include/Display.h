#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long lastRefreshMillis = 0;

class Display {
 private:
  static const int refreshIntervalMillis = 20;

 public:
  static void begin() {
    bool success = display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

    if (!success) {
      Serial.println("[!] Failed to initialize display");
    }

    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
  }

  static void render(const char* text, uint8_t line = 0, boolean push = false) {
    uint8_t textSize = 8;
    uint8_t yCursorStart = 5 + line * textSize;
    display.setCursor(0, yCursorStart);
    display.fillRect(0, yCursorStart, SCREEN_WIDTH, textSize, SSD1306_BLACK);
    display.println(text);
    if (push)
      display.display();
  }

  static void handle() {
    auto currentMillis = millis();
    if (currentMillis - lastRefreshMillis < refreshIntervalMillis)
      return;
    lastRefreshMillis = millis();

    display.display();
  }
};