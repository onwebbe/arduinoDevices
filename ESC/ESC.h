#ifndef _ESC_CONTROLLER_H__
#define _ESC_CONTROLLER_H__
#include <Arduino.h>
#include <Servo.h>

class ESC8266 {
  public:
    int _maxSpeed;
    int _minSpeed;
    int _currentSpeed;
    int _pin;
    Servo* _servo;
    int _gap;
  public:
    ESC8266(int pin);
    ESC8266(int pin, int gap);
    ~ESC8266();
    void init(int max, int min);
    void init();
    void changeSpeed(int speed);
    boolean increase();
    boolean decrease();
};
#endif
