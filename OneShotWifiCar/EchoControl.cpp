#include <Adafruit_PCF8574.h>
#include "EchoControl.h"

EchoControl::EchoControl(int trigPin, int echoPin) {
  init(trigPin, echoPin);
}

void EchoControl::init(int trigPin, int echoPin) {
  _trigPin = trigPin;
  _echoPin = echoPin;
  _pcf = new Adafruit_PCF8574();
  _pcf->begin(0x20, &Wire);
}

void EchoControl::setup() {
  _pcf->pinMode(_trigPin, OUTPUT);
  pinMode(_echoPin, INPUT);
}

float EchoControl::getDistance() {
  _pcf->digitalWrite(_trigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin 
  delayMicroseconds(2);       // delayMicroseconds在更小的时间内延时准确
  _pcf->digitalWrite(_trigPin, HIGH); 
  delayMicroseconds(10); 
  _pcf->digitalWrite(_trigPin, LOW); //通过这里控制超声波的发射
  
  float cm = pulseIn(_echoPin, HIGH) / 58.0; //将回波时间换算成cm 
  cm = (int(cm * 100.0)) / 100.0; //保留两位小数 
  return cm;
}