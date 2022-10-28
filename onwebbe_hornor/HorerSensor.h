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
    bool newDataComming = false;
    unsigned long _latestGap;
    long _gapList[20];
    long _startTime;
    long _endTime;
    long _timeout = 15000;
  private:
    void autoPopulateZeroGap();
  public:
    HorerSensor(uint8_t horerPin);
    ~HorerSensor();
    void init();
    void addGap(long gap);
    float getAverageRPM();
    float getLatestRPM();
    long* getAllGaps();
};
#endif