

#ifndef __BMP180_8266_H
#define __BMP180_8266_H

class BMP180_8266 {
  public:
    static bool begin();
    static float getPressure();
};

#endif
