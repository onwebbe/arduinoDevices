#include "HornerSensor_8266.h"
#include "Arduino.h"

ICACHE_RAM_ATTR void isrx(void* arg) {
  // Serial.println("isrx start");
  HorerSensor_8266* s = static_cast<HorerSensor_8266*>(arg);

  s->_endTime = millis();
  
  long gap = s->_endTime - s->_startTime;
  // Serial.printf("isrx gap:%d", gap);
  // almost 50 days, the timer will reset to 0 so reset myTime1 to sync with timer reset
  if (gap < 0) {
    s->_startTime = millis();
  }
  // 15s _timeout
  if (gap > s->_timeout) {
    s->_startTime = millis();
  }
  if (gap > 200) {
    // s->numberSensorHits += 1;
    s->_latestGap = gap;
    s->_startTime = millis();
    s->addGap(gap);
//    if (!isnan(gap) && gap > 0) {
//      Serial.printf("rpm: %u\n", 30000 / gap);
//    }
  }
}

HorerSensor_8266::HorerSensor_8266(uint8_t horerPin) {
  PIN = horerPin;
}
void HorerSensor_8266::setup() {
  pinMode(PIN, INPUT_PULLUP);
  attachInterruptArg(PIN, isrx, this, FALLING);
  _startTime = millis();
  for(int i = 0; i < 20; i++) {
    this->_gapList[i] = 60000;
  }
  Serial.println("_gapListInitialized");
}
HorerSensor_8266::~HorerSensor_8266() {
}
void HorerSensor_8266::addGap(long gap) {
  // Serial.printf("start to add Gap:%d\n", gap);
  for(int i = 1; i < 20; i++) {
    this->_gapList[i - 1] = this->_gapList[i];
  }
  _gapList[19] = gap;
}
float HorerSensor_8266::getAverageRPM() {
  // Serial.println("getAverageRPM ");
  // for(int i = 0; i < 20; i++) {
  //   Serial.println(this->_gapList[i]);
  // }
  autoPopulateZeroGap();
  long totalGap = 0;
  float totalGapCount = 0;
  for(int i = 17; i < 20; i++) {
    if (this->_gapList[i] < 60000) {
      totalGap += this->_gapList[i];
      totalGapCount++;
      // Serial.printf("currentGap:%d totalGap:%d",this->_gapList[i], totalGap);
    }
  }
  if (totalGapCount > 0) {
    float calculatedGap = totalGap / totalGapCount;
    // Serial.printf("totalGap:%lf gapCount:%lf calculted:%lf\n", totalGap, totalGapCount, calculatedGap);
    if (calculatedGap > 0) {
      return 15000 / calculatedGap;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}
float HorerSensor_8266::getLatestRPM() {
  autoPopulateZeroGap();
  if (_latestGap > 0 && _latestGap < 60000) {
    return 15000.0 / _latestGap;
  }
  return 0;
}
void HorerSensor_8266::autoPopulateZeroGap() {
  _endTime = millis();
  long gap = _endTime - _startTime;
  // Serial.printf("start autoPopulateZeroGap:%d - %d = %d _timeout:%d\n", _endTime, _startTime, gap, _timeout);
  if (gap > _timeout) {
    // Serial.println("autoPopulateZeroGap _timeout, add 0");
    int roundGapValue = floor((float)gap / _timeout);
    for(int i = 0; i < roundGapValue; i++) {
      addGap(60000);
    }
    _startTime = millis();
  }
  // Serial.println("end autoPopulateZeroGap");
}
long* HorerSensor_8266::getAllGaps() {
  return _gapList;
}
