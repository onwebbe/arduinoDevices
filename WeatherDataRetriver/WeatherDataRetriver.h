#ifndef _WEATHER_DATA_RETRIVER_H__
#define _WEATHER_DATA_RETRIVER_H__
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <ArduinoJson.h>
#include "HorerSensor.h"
#include <WeatherDisplay.h>

class WeatherDataRetriver {
  private:
    uint8_t _dht22Pin;
    uint8_t _hornerSensorPin;
    uint8_t _rainDropPin;
    DHT* _dht;
    Adafruit_BMP085 _bmp180;
    HorerSensor* _horner;

    WeatherDisplay* _weatherDisplay;
  private:
    int getGY30Data();
    
  public:
    WeatherDataRetriver(int dht22Pin, int hornerPin, int rainDropPin);
    ~WeatherDataRetriver();
    void init();
    String getWeatherData();
    float getTemperature();
    float getHumidity();
    float getPressure();
    float getWindSpeed();
    boolean isRain();
};
#endif
