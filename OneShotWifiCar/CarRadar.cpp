#include <Adafruit_PCF8574.h>
#include "CarRadar.h"
#include "EchoControl.h"
#include "Arduino.h"

CarRadar::CarRadar(int trigPin, int echoPin, int servoPin) {
  init(trigPin, echoPin, servoPin);
}
void CarRadar::init(int trigPin, int echoPin, int servoPin) {
  _radarData = new float[18];
  _trigPin = trigPin;
  _echoPin = echoPin;
  _servoPin = servoPin;

  int freq = 50;      // 频率(20ms周期)
  int channel = 8;    // 通道(高速通道（0 ~ 7）由80MHz时钟驱动，低速通道（8 ~ 15）由 1MHz 时钟驱动。)
  int resolution = 8; // 分辨率

  ledcSetup(channel, freq, resolution);
  ledcAttachPin(_servoPin, channel);

  _echo = new EchoControl(_trigPin, _echoPin);

  _lastTime = millis();
}

int CarRadar::calculatePWM(int degree) {
  //0-180度
  //20ms周期，高电平0.5-2.5ms，对应0-180度角度
  const float deadZone = 6.4;//对应0.5ms（0.5ms/(20ms/256）)
  const float max = 32;//对应2.5ms
  if (degree < 0)
    degree = 0;
  if (degree > 180)
    degree = 180;
  return (int)(((max - deadZone) / 180) * degree + deadZone);
}

void CarRadar::scan(int angle) {
  ledcWrite(8, calculatePWM(angle));
  float distance = _echo->getDistance();
  _radarData[angle] = distance;
}

void CarRadar::loop() {
  long currentTime = millis();
  if ((currentTime - _lastTime) > _timeGap || (currentTime - _lastTime) < 0) {
    scan(_currentAngle);
    if (_isGo) {
      _currentAngle += 10;
      if (_currentAngle > 180) {
        _currentAngle -= 20;
        _isGo = false;
      }
    } else {
      _currentAngle -= 10;
      if (_currentAngle < 0) {
        _currentAngle += 20;
        _isGo = true;
      }
    }
    _lastTime = millis();
  }
}