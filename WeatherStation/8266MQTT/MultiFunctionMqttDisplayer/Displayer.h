#ifndef _DISPLAYER_H__
#define _DISPLAYER_H__
#include "SSD1306Wire.h"
class Displayer {
  private:
    SSD1306Wire* _displayer;
    double _temperature;
    double _humidity;
    char _light[5];
    char _wind[5];
    char _pressure[6];
    char _isRain[3];
    boolean isWeatherDataReceived = false;

    int _hetunBright;
    int _hetunPumb;
    bool _hetunHeater;
    double _hetunTemperature;

    int _leilongBright;
    int _leilongPumb;
    bool _leilongHeater;
    double _leilongTemperature;
  private:
    void drawTextImage(const uint8_t *bitmap, uint8_t left, uint8_t top);
    void drawActiveInactiveIcon(const uint8_t *bitmap, uint8_t left, uint8_t top);
    int currentPage = 0;
    
  public:
    Displayer();
    void init();
    void drarwTitle(String title, const char *rain, char *wind);
    void drawLineOne(const char *temperature, const char *humidity);
    void drawLineTwo(char *light, char *pressure);
    void drawLineThree(char *pressure, const char *rain);
    void setWeatherData(double temperature, double humidity, double light, double wind, double pressure, const char *rain);
    void displayConnecting();
    void displayWifiInfo();

    void loop();
    void drawPaginator();
    void displayWeather();

    void displayYugangHeTun();
    void displayYugangLeilong();

    void setYugangHeTunLight(int light);
    void setYugangHeTunPump(int pumb);
    void setYugangHeTunHeater(bool heat);
    void setYugangHeTunTemperature(double temperature);

    void setYugangLeiLongLight(int light);
    void setYugangLeiLongPump(int pumb);
    void setYugangLeiLongHeater(bool heat);
    void setYugangLeiLongTemperature(double temperature);

    void nextPage();
    void prevPage();
};
#endif
