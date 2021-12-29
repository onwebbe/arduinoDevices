#include <Arduino.h>
#include <Servo.h>
#include "ESC.h"

ESC8266::ESC8266(int pin) {
  _pin = pin;
  _gap = 5;
  _servo = new Servo();
  _servo->attach(pin, 1000, 2000);
}
ESC8266::ESC8266(int pin, int gap) {
  _pin = pin;
  _gap = gap;
  _servo = new Servo();
  _servo->attach(pin, 1000, 2000);
}
ESC8266::~ESC8266() {
  delete _servo;
}
void ESC8266::init(int max, int min) {
  _maxSpeed = max;
  _minSpeed = min;
  _servo->write(max);
  delay(5000);
  _servo->write(min);
  delay(5000);
}
void ESC8266::init() {
  _maxSpeed = 180;
  _minSpeed = 0;
  _servo->write(180);
  delay(5000);
  _servo->write(0);
  delay(5000);
}
void ESC8266::changeSpeed(int speed) {
  if (speed <= _maxSpeed && speed >= _minSpeed) {
    _currentSpeed = speed;
    _servo->write(speed);
  }
}

boolean ESC8266::increase() {
  int speed = _currentSpeed + _gap;
  if (speed <= _maxSpeed && speed >= _minSpeed) {
    _currentSpeed = speed;
    _servo->write(_currentSpeed);
    return true;
  } else {
    return false;
  }
}

boolean ESC8266::decrease() {
  int speed = _currentSpeed - _gap;
  if (speed <= _maxSpeed && speed >= _minSpeed) {
    _currentSpeed = speed;
    _servo->write(_currentSpeed);
    return true;
  } else {
    return false;
  }
}
