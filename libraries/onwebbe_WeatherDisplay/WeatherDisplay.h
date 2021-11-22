
/*******************
Radar.h
Radar driver
*******************/
#ifndef _WEATHER_DISPLAY_H__
#define _WEATHER_DISPLAY_H__
#include <SPI.h>
class WeatherDisplay {
  private:
    float _temperature;
    int humidity;
    int light;
    int wind;
    int pressure;
    int isRain;
  private:
    void drawTextImage(const uint8_t *bitmap, uint8_t left, uint8_t top);
    
    
  public:
    WeatherDisplay();
    void init();
    void drawLineOne(float temperature, int humidity);
    void drawLineTwo(int light, int wind);
    void drawLineThree(int pressure, int rain);
    void displayData(float temperature, int humidity, int light, int wind, int pressure, int rain);
};
#endif
