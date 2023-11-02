#include "Radar8266.h"
#include "Arduino.h"
#include <Servo.h>
Servo svsv;
Radar8266::Radar8266(int trigPin, int echoPin, int motorPin) {
  this->srf05 = SRF05(trigPin, echoPin);
  _radarDataGo = new float[18];
  _radarDataBack = new float[18];
  // this->motorPin = motorPin;
  //Servo sv;
  //sv.attach(motorPin);
  this->servo = &svsv;
  this->servo->attach(motorPin);
  _motorPin = motorPin;

  Serial.println("Radar constructor:trig:" + String(trigPin)
      + ":echo:" + String(echoPin)
      + ":motor:" + String(motorPin));
}

void Radar8266::scan(int angle) {
  float distance = srf05.getDistance();
  _radarDataGo[angle] = distance;
}

void Radar8266::scan() {
  // svsv.attach(this->motorPin);
  for(int i = 0; i < 18; i++) {
    servo->write(i * 10);
    // analogWrite(_motorPin, calculatePWM(i * 10));
    float distance = srf05.getDistance();
    _radarDataGo[i] = distance;
    // Serial.println("serve:" + String(i * 10) + ":distance:" + String(distance));
    delay(100);
  }
  for(int i = 17; i >= 0; i--) {
    servo->write(i * 10);
    // analogWrite(_motorPin, calculatePWM(i * 10));
    float distance = srf05.getDistance();
    _radarDataBack[18 - i] = distance;
    // Serial.println("serve:" + String(i * 10) + ":distance:" + String(distance));
    delay(100);
  }
}

void Radar8266::rotate(int angle) {
  servo->write(angle);
}
void Radar8266::rotateAndScan(int angle) {
  Serial.println("start rotateAndScan");
  servo->write(angle);
  float distance = srf05.getDistance();
  Serial.println(distance);
  _radarDataGo[angle / 10] = distance;
  Serial.printf("angle: %d, distance: %d", angle, distance);
}

float* Radar8266::getRandarDataGo() {
  return _radarDataGo;
}

float* Radar8266::getRandarDataBack() {
  return _radarDataBack;
}

int Radar8266::calculatePWM(int degree)
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
