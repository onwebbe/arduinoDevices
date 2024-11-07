#include <Arduino.h>
#include <Wire.h>
#include "GY30_8266.h"

const uint8_t GY30_ADDR = 0b0100011;

GY30_8266::GY30_8266() {}
GY30_8266::~GY30_8266() {}
bool GY30_8266::setup() {
  Serial.println("start GY30");
  Wire.beginTransmission(GY30_ADDR);
  Wire.write(0b00000001);
  Wire.endTransmission();
  return Wire.endTransmission() == 0;
}
int GY30_8266::getGY30Data() {
  // put your main code here, to run repeatedly:
  int val = 0;
  // reset
  Wire.beginTransmission(GY30_ADDR);
  Wire.write(0b00000111);
  Wire.endTransmission();
 
  Wire.beginTransmission(GY30_ADDR);
  Wire.write(0b00100000);
  Wire.endTransmission();
  // typical read delay 120ms
  delay(120);
  Wire.requestFrom(GY30_ADDR, (uint8_t)2); // 2byte every time
  for (val = 0; Wire.available() >= 1; ) {
    char c = Wire.read();
    //Serial.println(c, HEX);
    val = (val << 8) + (c & 0xFF);
  }
  val = val / 1.2;
  Serial.print("lx: ");
  Serial.println(val);
  Serial.println("OK");
  return val;
}
