#include "Rain_8266.h"
#include "Arduino.h"

Rain_8266::Rain_8266(uint8_t rainPin) {
  PIN = rainPin;
  pinMode(PIN, INPUT);
}
bool Rain_8266::isRain() {
  int isRainDrop = digitalRead(PIN);
  Serial.println("isRain:");
  Serial.println(isRainDrop);
  return isRainDrop == 1 ? false: true;
}
Rain_8266::~Rain_8266() {
}
