/*****************
SRF05.cpp

******************/
#include "SRF05.h"
#include "Arduino.h"
SRF05::SRF05(int trigPin, int echoPin) {
  this->_trigPin = trigPin;
  this->_echoPin = echoPin;
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

SRF05::SRF05() {
}

SRF05::~SRF05() {}

float SRF05::getDistance() {
  digitalWrite(this->_trigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin 
  delayMicroseconds(2);       // delayMicroseconds在更小的时间内延时准确
  digitalWrite(this->_trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(this->_trigPin, LOW); //通过这里控制超声波的发射
  
  float cm = pulseIn(this->_echoPin, HIGH) / 58.0; //将回波时间换算成cm 
  cm = (int(cm * 100.0)) / 100.0; //保留两位小数 
  return cm;
}
