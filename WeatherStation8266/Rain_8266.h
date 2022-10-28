#include "Arduino.h"
#include <LinkedList.h>
#include <FS.h>
/*******************
HorerSensor.h
HorerSensor driver
*******************/
#ifndef _RAIN_8266_H__
#define _RAIN_8266_H__
class Rain_8266 {
  public:
  private:
    uint8_t PIN;
  public:
    Rain_8266(uint8_t rainPin);
    ~Rain_8266();
    bool isRain();
};
#endif
