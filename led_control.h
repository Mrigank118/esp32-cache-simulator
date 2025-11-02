#pragma once
#include <Arduino.h>

class LEDControl {
public:
  void begin(uint8_t hitPin, uint8_t missPin, uint8_t buzzerPin);
  void hit();
  void miss();

private:
  uint8_t hitPin, missPin, buzzerPin;
};
