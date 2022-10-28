#include <Arduino.h>
#include "HorerSensor8266.h"
HorerSensor8266 sensor = HorerSensor8266(16);
void setup() {
    Serial.begin(9600);
}

void loop() {
  delay(2000);
  Serial.printf("----avg start---- %f ----avg end----\n", sensor.getAverageRPM());
}

//struct Horor {
//    const uint8_t PIN;
//    uint32_t numberKeyPresses;
//    bool newDataComming;
//    unsigned long gap;
//};
//
//Horor button1 = {16, 0, false, 0};
//unsigned long myTime1;
//unsigned long myTime2;
//void ARDUINO_ISR_ATTR isr(void* arg) {
//  Horor* s = static_cast<Horor*>(arg);
//
//  myTime2 = millis();
//  unsigned long gap = myTime2 - myTime1;
//  // almost 50 days, the timer will reset to 0 so reset myTime1 to sync with timer reset
//  if (gap < 0) {
//    myTime1 = millis();
//  }
//  // 15s timeout
//  if (gap > 15000) {
//    myTime1 = millis();
//  }
//  if (gap > 200 && !s->newDataComming) {
//    s->numberKeyPresses += 1;
//    s->gap = gap;
//    myTime1 = millis();
//    if (!isnan(gap) && gap > 0) {
//      Serial.printf("rpm: %u\n", 30000 / gap);
//    }
//  }
//  s->newDataComming = !s->newDataComming;
//}
//
//void setup() {
//    Serial.begin(9600);
//    pinMode(button1.PIN, INPUT_PULLUP);
//    attachInterruptArg(button1.PIN, isr, &button1, FALLING);
//    myTime1 = millis();
//}
//
//void loop() {
//  
//  
//  delay(2000);
//}
