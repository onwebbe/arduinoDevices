
/*******************
SRF05.h
Distance Sensor
*******************/
#ifndef _SRF05_H__
#define _SRF05_H__

#include"Arduino.h"
class SRF05 {
  private:
    int _trigPin;
    int _echoPin;
  public:
    SRF05();
    SRF05(int trigPin, int echoPin);
    ~SRF05();
    float getDistance();
};
#endif
