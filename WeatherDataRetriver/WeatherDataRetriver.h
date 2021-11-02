#ifndef _WEATHER_DATA_RETRIVER_H__
#define _WEATHER_DATA_RETRIVER_H__
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <ArduinoJson.h>
class WeatherDataRetriver {
  private:
    int _dht22Pin;
    DHT* _dht;
    Adafruit_BMP085 _bmp180;
  private:
    int getGY30Data();
    
  public:
    WeatherDataRetriver(uint8_t dht22Pin);
    ~WeatherDataRetriver();
    void init();
    String getWeatherData();
    float getTemperature();
    float getHumidity();
    float getPressure();
};
#endif
