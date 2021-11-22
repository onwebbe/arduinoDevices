
/*******************
Radar.h
Radar driver
*******************/
#ifndef _RADAR_H__
#define _RADAR_H__
#include <SRF05.h>
// #include <Servo.h>
class Radar {
  private:
    float* _radarDataGo;
    float* _radarDataBack;
    SRF05 srf05;
    // Servo *servo;
    int freq;      // 频率(20ms周期)
    int channel;    // 通道(高速通道（0 ~ 7）由80MHz时钟驱动，低速通道（8 ~ 15）由 1MHz 时钟驱动。)
    int resolution; // 分辨率
    int led;
    int _motorPin;
  private:
    int calculatePWM(int degree);
    
  public:
    Radar(int trigPin, int echoPin, int motorPin);
    void scan(int angle);
    void scan();
    float* getRandarDataGo();
    float* getRandarDataBack();
};
#endif
