
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
    void drawLineOne(char *temperature, char *humidity);
    void drawLineTwo(char *light, char *wind);
    void drawLineThree(char *pressure, char *rain);
    void displayData(char *temperature, char *humidity, char *light, char *wind, char *pressure, char *rain);
    void displayConnecting();
    void displayWifiInfo();
};
#endif
