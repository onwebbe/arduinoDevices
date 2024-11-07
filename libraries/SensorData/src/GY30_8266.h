#include "Arduino.h"
#include <LinkedList.h>
/*******************
GY30_8266.h
GY30 driver
*******************/
#ifndef _GY30_8266_H__
#define _GY30_8266_H__
class GY30_8266 {
  public:
    GY30_8266();
    ~GY30_8266();
    bool setup();
    int getGY30Data();
};
#endif
