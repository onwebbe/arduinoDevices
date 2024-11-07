#include <Arduino.h>
#include <Wire.h>
#include "BMP180_8266.h"
#include <Adafruit_BMP085.h>

BMP180_8266::BMP180_8266() {}
BMP180_8266::~BMP180_8266() {}

bool BMP180_8266::setup() {
  bmp180 = new Adafruit_BMP085();
  return bmp180->begin();
}


float BMP180_8266::getPressure() {
  float presure = bmp180->readPressure() / 100.0;
  return presure;
}
