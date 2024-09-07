#include <Adafruit_PCF8574.h>

#ifndef _HA_WIFI_CAR_DRIVER_CONTROL_H__
#define _HA_WIFI_CAR_DRIVER_CONTROL_H__

class DriverControl {
  private:
    int _leftPinA;
    int _leftPinB;
    int _rightPinA;
    int _rightPinB;

    int _enLeft;
    int _enRight;

    bool _leftPinAPCF = false;
    bool _leftPinBPCF = false;
    bool _rightPinAPCF = false;
    bool _rightPinBPCF = false;
    bool _enLeftPCF = false;
    bool _enRightPCF = false;

    int _maxSpeedNum = 255;

    

    Adafruit_PCF8574 *_pcf;
  public:
    DriverControl(Adafruit_PCF8574 *pcf);
    DriverControl(Adafruit_PCF8574 *pcf, int leftA, int leftB, int rightA, int rightB, int enLeft, int enRight);
    DriverControl(Adafruit_PCF8574 *pcf, int leftA, int leftB, int rightA, int rightB, int enLeft, int enRight, int maxSpeedNum);
    void init(Adafruit_PCF8574 *pcf, int leftA, int leftB, int rightA, int rightB, int enLeft, int enRight, int maxSpeedNum);
    void setup();
    void setLeftAPCF(bool isPCF);
    void setLeftBPCF(bool isPCF);
    void setRightAPCF(bool isPCF);
    void setRightBPCF(bool isPCF);
    void setENLeftPCF(bool isPCF);
    void setENRightPCF(bool isPCF);

    void goStaight(int speed);
    void goBack(int speed);
    void turnLeft(int speed);
    void turnRight(int speed);
    void turn(int leftA, int leftB, int rightA, int rightB, int enLeft, int enRight);
};
#endif