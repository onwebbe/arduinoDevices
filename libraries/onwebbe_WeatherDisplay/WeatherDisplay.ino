#include "WeatherDisplay.h"
WeatherDisplay weatherDisplay = WeatherDisplay();
void setup()   {
  weatherDisplay.init();
}


void loop() {
  double temperature = random(100);
  temperature = temperature / 10;

  int humidity = random(100);

  int light = random(20000);

  int wind = random(60);

  int pressure = random(15000);

  int rain = random(1);
  weatherDisplay.displayData(temperature, humidity, light, wind, pressure, rain);
  delay(10000);
}
