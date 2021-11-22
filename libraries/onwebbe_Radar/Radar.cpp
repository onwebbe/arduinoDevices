#include "Radar.h"
#include "Arduino.h"
//#include <Servo.h>
//Servo svsv;

Radar::Radar(int trigPin, int echoPin, int motorPin) {
  this->srf05 = SRF05(trigPin, echoPin);
  _radarDataGo = new float[18];
  _radarDataBack = new float[18];
  // this->motorPin = motorPin;
  //Servo sv;
  //sv.attach(motorPin);
  //this->servo = &sv;
  freq = 50;      // 频率(20ms周期)
  channel = 8;    // 通道(高速通道（0 ~ 7）由80MHz时钟驱动，低速通道（8 ~ 15）由 1MHz 时钟驱动。)
  resolution = 8; // 分辨率
  _motorPin = motorPin;

  ledcSetup(channel, freq, resolution);
  ledcAttachPin(motorPin, channel); 
  Serial.println("Radar constructor:trig:" + String(trigPin)
      + ":echo:" + String(echoPin)
      + ":motor:" + String(motorPin));
}

void Radar::scan(int angle) {
  float distance = srf05.getDistance();
  _radarDataGo[angle] = distance;
}

void Radar::scan() {
  // svsv.attach(this->motorPin);
  for(int i = 0; i < 18; i++) {
    // svsv.write(i * 10);
    ledcWrite(channel, calculatePWM(i * 10));
    float distance = srf05.getDistance();
    _radarDataGo[i] = distance;
    // Serial.println("serve:" + String(i * 10) + ":distance:" + String(distance));
    delay(100);
  }
  for(int i = 17; i >= 0; i--) {
    // svsv.write(i * 10);
    ledcWrite(channel, calculatePWM(i * 10));
    float distance = srf05.getDistance();
    _radarDataBack[18 - i] = distance;
    // Serial.println("serve:" + String(i * 10) + ":distance:" + String(distance));
    delay(100);
  }
}

float* Radar::getRandarDataGo() {
  return _radarDataGo;
}

float* Radar::getRandarDataBack() {
  return _radarDataBack;
}

int Radar::calculatePWM(int degree)
{ //0-180度
 //20ms周期，高电平0.5-2.5ms，对应0-180度角度
  const float deadZone = 6.4;//对应0.5ms（0.5ms/(20ms/256）)
  const float max = 32;//对应2.5ms
  if (degree < 0)
    degree = 0;
  if (degree > 180)
    degree = 180;
  return (int)(((max - deadZone) / 180) * degree + deadZone);
}
