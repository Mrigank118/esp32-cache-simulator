#include "led_control.h"

void LEDControl::begin(uint8_t hPin, uint8_t mPin, uint8_t bPin) {
  hitPin = hPin;
  missPin = mPin;
  buzzerPin = bPin;

  pinMode(hitPin, OUTPUT);
  pinMode(missPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(hitPin, LOW);
  digitalWrite(missPin, LOW);
  digitalWrite(buzzerPin, LOW);
}

void LEDControl::hit() {
  digitalWrite(hitPin, HIGH);
  digitalWrite(missPin, LOW);

  // 🔊 short beep for HIT
  digitalWrite(buzzerPin, HIGH);
  delay(80);
  digitalWrite(buzzerPin, LOW);

  delay(80);
  digitalWrite(hitPin, LOW);
}

void LEDControl::miss() {
  digitalWrite(missPin, HIGH);
  digitalWrite(hitPin, LOW);
  delay(100);
  digitalWrite(missPin, LOW);
}
