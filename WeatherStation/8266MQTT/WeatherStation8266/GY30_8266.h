#include "Arduino.h"
#include <LinkedList.h>
/*******************
HorerSensor.h
HorerSensor driver
*******************/
#ifndef _GY30_8266_H__
#define _GY30_8266_H__
class GY30_8266 {
  public:
  private:
  public:
    GY30_8266();
    ~GY30_8266();
    static bool begin();
    static int getGY30Data();
};
#endif
