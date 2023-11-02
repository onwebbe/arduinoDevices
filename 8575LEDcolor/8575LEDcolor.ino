#include "PCF8575.h"
#include "Arduino.h"
int _correctionFactor = 1;
PCF8575 pcf8575(0x20);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pcf8575.pinMode(0, OUTPUT);
  pcf8575.pinMode(1, OUTPUT);
  pcf8575.pinMode(2, OUTPUT);
  pcf8575.begin();
  pcf8575.digitalWrite(0, LOW);
  pcf8575.digitalWrite(1, LOW);
  pcf8575.digitalWrite(2, LOW);

}

void loop() {
//  analogWrite(12, 100); 
//  analogWrite(13, 0);
//  analogWrite(14, 100);
  pcf8575.digitalWrite(0, HIGH);
  delay(2000);
  pcf8575.digitalWrite(0, LOW);
  pcf8575.digitalWrite(1, HIGH);
  delay(2000);
  pcf8575.digitalWrite(1, LOW);
  pcf8575.digitalWrite(2, HIGH);
  delay(2000);
  pcf8575.digitalWrite(2, LOW);
  pcf8575.digitalWrite(0, HIGH);
  delay(2000);
//  digitalWrite(12,HIGH);
//  digitalWrite(13,HIGH);
//  delay(500);
//  digitalWrite(12,LOW);
//  digitalWrite(13,LOW);
//  delay(500);
//  digitalWrite(13,HIGH);
//  digitalWrite(14,HIGH);
//  delay(500);
//  digitalWrite(13,LOW);
//  digitalWrite(14,LOW);
//  delay(500);
//  digitalWrite(12,HIGH);
//  digitalWrite(14,HIGH);
//  delay(500);
//  digitalWrite(12,LOW);
//  digitalWrite(14,LOW);
//  delay(500);
//  digitalWrite(12,HIGH);
//  digitalWrite(13,HIGH);
//  digitalWrite(14,HIGH);
//  delay(500);
//  digitalWrite(12,LOW);
//  digitalWrite(13,LOW);
//  digitalWrite(14,LOW);
//  delay(500);
  // put your main code here, to run repeatedly:

}
