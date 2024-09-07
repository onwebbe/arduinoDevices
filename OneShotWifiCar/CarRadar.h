#include <Adafruit_PCF8574.h>
#include "EchoControl.h"

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
    Adafruit_PCF8574 *_pcf;
    float* _radarData;
    int _timeGap = 100;
    long _lastTime;

    void init(Adafruit_PCF8574 *pcf, int trigPin, int echoPin, int servoPin);
    int calculatePWM(int degree);
    void scan(int angle);

  public:
    CarRadar(Adafruit_PCF8574 *pcf, int trigPin, int echoPin, int servoPin);
    void setup();
    void loop();
};
#endif