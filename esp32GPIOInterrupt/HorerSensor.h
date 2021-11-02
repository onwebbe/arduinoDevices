#include "Arduino.h"
#include <LinkedList.h>
/*******************
HorerSensor.h
HorerSensor driver
*******************/
#ifndef _HORERSENSOR_H__
#define _HORERSENSOR_H__
class HorerSensor {
  public:
    uint8_t PIN;
    uint32_t numberSensorHits;
    bool newDataComming;
    unsigned long latestGap;
    long gapList[20];
    long startTime;
    long endTime;
    long timeout = 15000;
  private:
    void autoPopulateZeroGap();
  public:
    HorerSensor(uint8_t horerPin);
    ~HorerSensor();
    void addGap(long gap);
    float getAverageRPM();
    float getLatestRPM();
};
#endif
