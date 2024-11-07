#ifndef __BMP180_8266_H
#define __BMP180_8266_H
#include <Adafruit_BMP085.h>

class BMP180_8266 {
  private:
    Adafruit_BMP085 *bmp180;
  public:
    BMP180_8266();
    ~BMP180_8266();
    bool setup();
    float getPressure();
};

#endif
