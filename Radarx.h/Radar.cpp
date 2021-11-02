#include "Radar.h"
#include <Servo.h>
Servo servo;
Radar::Radar(int trigPin, int echoPin, int motorPin) {
  this._trigPin = trigPin;
  this._echoPin = echoPin;
  this._motorPin = motorPin;
}
void begin() {
  // put your setup code here, to run once:
  pinMode(this._trigPin, OUTPUT); 
  pinMode(this._echoPin, INPUT);
  servo.attach(this._motorPin);
}

void scan() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < this._radarData.length; i++) {
    float cm;
    int angle = -90 + (i * 180 / this._radarData.length);
    servo.write(angle);
    digitalWrite(TrigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin 
    delayMicroseconds(2);       // delayMicroseconds在更小的时间内延时准确
    digitalWrite(TrigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(TrigPin, LOW); //通过这里控制超声波的发射
    
    cm = pulseIn(EchoPin, HIGH) / 58.0; //将回波时间换算成cm 
    cm = (int(cm * 100.0)) / 100.0; //保留两位小数 
    this._radarData.length = cm;
  }
}
