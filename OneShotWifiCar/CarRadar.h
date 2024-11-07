#include <Adafruit_PCF8574.h>
#include "EchoControl.h"
#include <ESP32Servo.h>

#ifndef _HA_WIFI_CAR_CARRADAR_H__
#define _HA_WIFI_CAR_CARRADAR_H__

class CarRadar {
  private:
    int _trigPin;
    int _echoPin;
    int _servoPin;
    int _currentAngle = 0;
    bool _isGo = true;
    EchoControl *_echo;
    Servo *_myservo;

    Adafruit_PCF8574 *_pcf;
    float* _radarData;
    int _timeGap = 300;
    long _lastTime;

    bool _trigPinAPCF = false;

    void init(Adafruit_PCF8574 *pcf, int trigPin, int echoPin, int servoPin);
    int calculatePWM(int degree);
    void scan(int angle);

  public:
    CarRadar(Adafruit_PCF8574 *pcf, int trigPin, int echoPin, int servoPin);
    void setup();
    void loop();

    void setTrigPinAPCF(bool isAPCF);
};
#endif