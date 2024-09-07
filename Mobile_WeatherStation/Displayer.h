#ifndef _DISPLAYER_H__
#define _DISPLAYER_H__
#include "SSD1306Wire.h"
class Displayer {
  private:
    SSD1306Wire* _displayer;
    double _temperature;
    double _humidity;
    char _pressure[7];
    boolean isWeatherDataReceived = false;

    int _hetunBright;
    int _hetunPumb;
    bool _hetunHeater;
    double _hetunTemperature;

    int _leilongBright;
    int _leilongPumb;
    bool _leilongHeater;
    double _leilongTemperature;

    bool _isLaugh = false;
  private:
    void drawTextImage(const uint8_t *bitmap, uint8_t left, uint8_t top);
    void drawActiveInactiveIcon(const uint8_t *bitmap, uint8_t left, uint8_t top);
    int currentPage = 0;
    
  public:
    Displayer();
    void init();
    void setLaugh(bool isLaugh);
    void drarwTitle(String title);
    void drawLineOne(const char *temperature, const char *humidity);
    void drawLineTwo(char *pressure);
    void setWeatherData(double temperature, double humidity, double pressure);
    void displayWeather();
    void loop();
};
#endif
