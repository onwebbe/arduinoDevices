

#ifndef __AHT10_8266_H
#define __AHT10_8266_H

class AHT10_8266{
public:
  static bool begin();
  static bool reset();
  static bool measure(float *temp, float *hum);
  static float getTemperature();
  static float getHumidity();
};

#endif
