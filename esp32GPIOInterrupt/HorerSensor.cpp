#include "HorerSensor.h"
#include "Arduino.h"

void ARDUINO_ISR_ATTR isrx(void* arg) {
  HorerSensor* s = static_cast<HorerSensor*>(arg);

  s->endTime = millis();
  
  unsigned long gap = s->endTime - s->startTime;
  // almost 50 days, the timer will reset to 0 so reset myTime1 to sync with timer reset
  if (gap < 0) {
    s->startTime = millis();
  }
  // 15s timeout
  if (gap > s->timeout) {
    s->startTime = millis();
  }
  if (gap > 200 && !s->newDataComming) {
    s->numberSensorHits += 1;
    s->latestGap = gap;
    s->startTime = millis();
    s->addGap(gap);
//    if (!isnan(gap) && gap > 0) {
//      Serial.printf("rpm: %u\n", 30000 / gap);
//    }
  }
  s->newDataComming = !s->newDataComming;
}

HorerSensor::HorerSensor(uint8_t horerPin) {
  PIN = horerPin;
  pinMode(PIN, INPUT_PULLUP);
}
void HorerSensor::init() {
  attachInterruptArg(PIN, isrx, this, FALLING);
  startTime = millis();
  for(int i = 0; i < 20; i++) {
    gapList[i] = -1;
  }
}
HorerSensor::~HorerSensor() {
}
void HorerSensor::addGap(long gap) {
  for(int i = 1; i < 20; i++) {
    gapList[i -1] = gapList[i];
  }
  gapList[19] = gap;
}
float HorerSensor::getAverageRPM() {
  autoPopulateZeroGap();
  float totalGap = 0;
  float totalGapCount = 0;
  for(int i = 14; i < 20; i++) {
    if (gapList[i] > 0) {
      totalGap += gapList[i];
      totalGapCount++;
      Serial.println(gapList[i]);
    }
  }
  if (totalGapCount > 0) {
    float calculatedGap = totalGap / totalGapCount;
    Serial.printf("totalGap:%lf gapCount:%lf calculted:%lf\n", totalGap, totalGapCount, calculatedGap);
    if (calculatedGap > 0) {
      return 30000 / calculatedGap;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}
float HorerSensor::getLatestRPM() {
  autoPopulateZeroGap();
  if (latestGap > 0) {
    return 30000.0 / latestGap;
  }
  return 0;
}
void HorerSensor::autoPopulateZeroGap() {
  Serial.println("start autoPopulateZeroGap");
  endTime = millis();
  int gap = endTime - startTime;
  if (gap > timeout) {
    int roundGapValue = floor((float)gap / timeout);
    for(int i = 0; i < roundGapValue; i++) {
      addGap(0);
    }
    startTime = millis();
  }
  Serial.println("end autoPopulateZeroGap");
}
