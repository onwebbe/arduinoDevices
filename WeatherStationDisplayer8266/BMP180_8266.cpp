#include <Arduino.h>
#include <Wire.h>
#include "BMP180_8266.h"
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp180;

bool BMP180_8266::begin() {
  return bmp180.begin();
}


float BMP180_8266::getPressure() {
  float presure = bmp180.readPressure() / 100.0;
  return presure;
}
