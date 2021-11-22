#include "WeatherDisplay.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM temperatureIcon[] =
{ B00000000, B00000000,
  B00000111, B11000000,
  B00000100, B01000000,
  B00000111, B01000000,
  B00000100, B01000000,
  B00000111, B01000000,
  B00000100, B01000000,
  B00000111, B01000000,
  B00000100, B01000000,
  B00000100, B01000000,
  B00000111, B11000000,
  B00001111, B11100000,
  B00001111, B11100000,
  B00001111, B11100000,
  B00000111, B11000000,
  B00000011, B10000000
};
static const unsigned char PROGMEM lightIcon[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00011111, B11110000,
  B00011110, B00010000,
  B00011110, B00010000,
  B00011110, B00010000,
  B00011110, B00010000,
  B00011110, B00010000,
  B00011110, B00010000,
  B00011110, B00010000,
  B00011110, B00010000,
  B00011110, B00010000,
  B00011110, B00010000,
  B00011111, B11110000,
  B00000000, B00000000,
  B00000000, B00000000
};
static const unsigned char PROGMEM pressureIcon[] =
{ B00000000, B00000000,
  B00000111, B11000000,
  B00000111, B11000000,
  B00000111, B11000000,
  B00000111, B11000000,
  B00000111, B11000000,
  B00000111, B11000000,
  B00000111, B11000000,
  B00000111, B11000000,
  B00011111, B11110000,
  B00001111, B11100000,
  B00000110, B11000000,
  B00000001, B00000000,
  B00111100, B01111000,
  B00000011, B10000000,
  B00000000, B00000000
};
static const unsigned char PROGMEM sunIcon[] =
{
  B00000001, B00000000,
  B00000001, B00000000,
  B00010001, B00010000,
  B00001011, B10100000,
  B00000111, B11100000,
  B00001111, B11100000,
  B01111111, B11111100,
  B00001111, B11100000,
  B00000111, B11000000,
  B00001011, B10100000,
  B00010001, B00010000,
  B00000001, B00000000,
  B00000001, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000
};
static const unsigned char PROGMEM rainIcon[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00001000, B00000000,
  B00011000, B00000000,
  B00111100, B00000000,
  B00011000, B00100000,
  B00000000, B01100000,
  B00000000, B11110000,
  B00000000, B01100000,
  B00000100, B00000000,
  B00001100, B00000000,
  B00011110, B00010000,
  B00001100, B00110000,
  B00000000, B01111000,
  B00000000, B00110000,
  B00000000, B00000000
};
#define ICON16_GLCD_HEIGHT 16
#define ICON16_GLCD_WIDTH  16
Adafruit_SH1106G _display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WeatherDisplay::WeatherDisplay() {
  
}
void WeatherDisplay::init() {
  _display.begin(i2c_Address, true);
  _display.setContrast(0);
  _display.display();
  delay(2000);
  _display.clearDisplay();
}
void WeatherDisplay::displayData(float temperature, int humidity, int light, int wind, int pressure, int rain) {
  _display.clearDisplay();
  this->drawLineOne(temperature, humidity);
  this->drawLineTwo(light, wind);
  this->drawLineThree(pressure, rain);
  _display.display();
}
void WeatherDisplay::drawTextImage(const uint8_t *bitmap, uint8_t left, uint8_t top) {
  _display.drawBitmap(left, top, bitmap, ICON16_GLCD_WIDTH, ICON16_GLCD_HEIGHT, SH110X_WHITE);
}
void WeatherDisplay::drawLineOne(float temperature, int humidity) {
  this->drawTextImage(temperatureIcon, 0, 25);
  _display.setTextSize(2);
  _display.setTextColor(SH110X_WHITE);
  _display.setCursor(16, 25);
  String temperatureStr = "";
  temperatureStr.concat(temperature);
  char b[5];
  temperatureStr = temperatureStr.substring(0, 5);
  strcpy(b,temperatureStr.c_str());
  b[4] = ' ';
  _display.write(b);

  String humidityStr = "";
  humidityStr.concat(humidity);
  humidityStr.concat("%");
  char c[4];
  strcpy(c,humidityStr.c_str());
  _display.setCursor(16 * 5, 25);
  _display.write(c);
}
void WeatherDisplay::drawLineTwo(int light, int wind) {
  this->drawTextImage(lightIcon, 0, 48);
  _display.setTextSize(2);
  _display.setTextColor(SH110X_WHITE);
  _display.setCursor(16, 48);
  String lightStr = "";
  lightStr.concat(light);
  char b[5];
  lightStr = lightStr.substring(0, 5);
  strcpy(b,lightStr.c_str());
  _display.write(b);

  String windStr = "";
  windStr.concat(wind);
  char c[4];
  strcpy(c,windStr.c_str());
  _display.setCursor(16 * 5, 48);
  _display.write(c);
}
void WeatherDisplay::drawLineThree(int pressure, int rain) {
  this->drawTextImage(pressureIcon, 0, 0);
  _display.setTextSize(2);
  _display.setTextColor(SH110X_WHITE);
  _display.setCursor(16, 0);
  String pressureStr = "";
  pressureStr.concat(pressure);
  char b[5];
  pressureStr = pressureStr.substring(0, 5);
  strcpy(b,pressureStr.c_str());
  _display.write(b);
  
  if (rain == 0) {
    this->drawTextImage(sunIcon, 16 * 5, 0);
  } else {
    this->drawTextImage(rainIcon, 16 * 5, 0);
  }
}
