#include "PCF8575.h"
#include "Arduino.h"
int _correctionFactor = 1;
PCF8575 pcf8575(0x20);
void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  
  pcf8575.pinMode(0, OUTPUT);
  pcf8575.pinMode(1, INPUT);
  pcf8575.digitalWrite(12, LOW);
  pcf8575.begin();
  pcf8575.pinMode(0, OUTPUT);
  pcf8575.pinMode(1, INPUT);
  pcf8575.digitalWrite(0, LOW);
}
int PurseIn8575(int pin) {
  long start = micros();
  long start2 = micros();
  long end = micros();
  while(true) {
    int readVal = pcf8575.digitalRead(pin);
    
    if (readVal == HIGH) {
      Serial.println("readVal: HIGH");
      start2 = micros();
      break;
    }
    start2 = micros();
    if ((start2 - start) >= 1000000) {
      break;
    }
    if (start2 < start) {
      start2 = start;
      break;
    }
  }
  if (start2 <= start) {
    return 0;
  }
  while(true) {
    int readVal = pcf8575.digitalRead(pin);
    if (readVal == LOW) {
      Serial.println("readVal: LOW");
      end = micros();
      break;
    }
    end = micros();
    if ((end - start2) >= 1000000) {
      break;
    }
    if (end < start2) {
      end = start2;
      break;
    }
  }
  if (end <= start2) {
    return 0;
  }
  return end - start;
}
void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 1000; i++) {
    pcf8575.digitalWrite(0, HIGH);
    delayMicroseconds(10);
    pcf8575.digitalWrite(0, LOW);
    delayMicroseconds(10);
  }
  uint32_t duration = PurseIn8575(1);
  if (_correctionFactor != 1)
  {
     duration = round(duration * _correctionFactor);
  }
  Serial.println(round(duration * 340 / 200));

//  uint32_t duration = PurseIn8575(1);
//  if (_correctionFactor != 1)
//  {
//     duration = round(duration * _correctionFactor);
//  }
//  Serial.println(round(duration * 340 / 200));
//  delayMicroseconds(10);
//  pcf8575.digitalWrite(0, LOW);
  
}
