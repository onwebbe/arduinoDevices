#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
#include "Displayer.h"
/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO

#define LOGO16_GLCD_HEIGHT 10
#define LOGO16_GLCD_WIDTH  32
//static const unsigned char PROGMEM temperatureIcon[] =
//{ B00000000, B00000000, 
//  B11000000, B00000111, 
//  B01000000, B00000100, 
//  B01000000, B00000111, 
//  B01000000, B00000100, 
//  B01000000, B00000100, 
//  B11000000, B00000111,
//  B11100000, B00001111, 
//  B11000000, B00000111, 
//  B10000000, B00000011
//};
//static const unsigned char PROGMEM lightIcon[] =
//{
//  B00000000, B00000000,
//  B11110000, B00011111,
//  B00010000, B00011110,
//  B00010000, B00011110,
//  B00010000, B00011110,
//  B00010000, B00011110,
//  B00010000, B00011110,
//  B00010000, B00011110,
//  B11110000, B00011111,
//  B00000000, B00000000,
//};
//static const unsigned char PROGMEM pressureIcon[] =
//{ B11000000, B00000111, 
//  B11000000, B00000111, 
//  B11000000, B00000111, 
//  B11000000, B00000111, 
//  B11110000, B00011111, 
//  B11100000, B00001111, 
//  B11000000, B00000110, 
//  B00000000, B00000001, 
//  B01111000, B00111100, 
//  B10000000, B00000011
//};
//static const unsigned char PROGMEM sunIcon[] =
//{
//  B00000000, B00000001, 
//  B00010000, B00010001, 
//  B10100000, B00001011, 
//  B11100000, B00000111, 
//  B11100000, B00001111, 
//  B11111100, B01111111, 
//  B11100000, B00001111, 
//  B11000000, B00000111, 
//  B10100000, B00001011, 
//  B00010000, B00010001
//};
//static const unsigned char PROGMEM rainIcon[] =
//{ B00000000, B00000000,
//  B00000000, B00001000, 
//  B00000000, B00011000, 
//  B00000000, B00111100, 
//  B00100000, B00011000, 
//  B01100000, B00000000, 
//  B11110000, B00000000, 
//  B01100000, B00000000, 
//  B00000000, B00000000
//};
static const unsigned char PROGMEM temperatureIcon[] =
{ B00000000, B00000000, 
  B11000000, B00000111, 
  B01000000, B00000100, 
  B01000000, B00000111, 
  B01000000, B00000100, 
  B01000000, B00000111, 
  B01000000, B00000100, 
  B01000000, B00000111, 
  B01000000, B00000100, 
  B01000000, B00000100, 
  B11000000, B00000111, 
  B11100000, B00001111, 
  B11100000, B00001111, 
  B11100000, B00001111, 
  B11000000, B00000111, 
  B10000000, B00000011
};
static const unsigned char PROGMEM lightIcon[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B11110000, B00011111,
  B00010000, B00011110,
  B00010000, B00011110,
  B00010000, B00011110,
  B00010000, B00011110,
  B00010000, B00011110,
  B00010000, B00011110,
  B00010000, B00011110,
  B00010000, B00011110,
  B00010000, B00011110,
  B00010000, B00011110,
  B11110000, B00011111,
  B00000000, B00000000,
  B00000000, B00000000
};
static const unsigned char PROGMEM pressureIcon[] =
{ B00000000, B00000000, 
  B11000000, B00000111, 
  B11000000, B00000111, 
  B11000000, B00000111, 
  B11000000, B00000111, 
  B11000000, B00000111, 
  B11000000, B00000111, 
  B11000000, B00000111, 
  B11000000, B00000111, 
  B11110000, B00011111, 
  B11100000, B00001111, 
  B11000000, B00000110, 
  B00000000, B00000001, 
  B01111000, B00111100, 
  B10000000, B00000011, 
  B00000000, B00000000
};
static const unsigned char PROGMEM sunIcon[] =
{
  B00000000, B00000001, 
  B00000000, B00000001, 
  B00010000, B00010001, 
  B10100000, B00001011, 
  B11100000, B00000111, 
  B11100000, B00001111, 
  B11111100, B01111111, 
  B11100000, B00001111, 
  B11000000, B00000111, 
  B10100000, B00001011, 
  B00010000, B00010001, 
  B00000000, B00000001, 
  B00000000, B00000001, 
  B00000000, B00000000, 
  B00000000, B00000000, 
  B00000000, B00000000
};
static const unsigned char PROGMEM rainIcon[] =
{
  B00000000, B00000000, 
  B00000000, B00000000, 
  B00000000, B00001000, 
  B00000000, B00011000, 
  B00000000, B00111100, 
  B00100000, B00011000, 
  B01100000, B00000000, 
  B11110000, B00000000, 
  B01100000, B00000000, 
  B00000000, B00000100, 
  B00000000, B00001100, 
  B00010000, B00011110, 
  B00110000, B00001100, 
  B01111000, B00000000, 
  B00110000, B00000000, 
  B00000000, B00000000
};

static const unsigned char PROGMEM faceLaugh[] =
{
  B00110000, B00001100,
  B11111110, B01111111,
  B00000010, B01000000,
  B00010010, B01001000,
  B00101010, B01010100,
  B01000110, B01100010,
  B00000010, B01000000,
  B00000010, B01000000,
  B00000010, B01000000,
  B00001010, B01010000,
  B00001010, B01010000,
  B00100010, B01000100,
  B11000010, B01000011,
  B00000010, B01000000,
  B11111110, B01111111,
  B00000000, B00000000
};

static const unsigned char PROGMEM faceNormal[] =
{
  B00110000, B00001100,
  B11111110, B01111111,
  B00000010, B01000000,
  B00010010, B01001000,
  B00101010, B01010100,
  B01000110, B01100010,
  B00110010, B01001100,
  B00000010, B01000000,
  B00000010, B01000000,
  B00000010, B01000000,
  B00000010, B01000000,
  B11100010, B01000111,
  B00000010, B01000000,
  B00000010, B01000000,
  B11111110, B01111111,
  B00000000, B00000000
};

#define ICON16_GLCD_HEIGHT 16
#define ICON16_GLCD_WIDTH  16

Displayer::Displayer() {
}
void Displayer::init() {
  _displayer = new SSD1306Wire(0x3c, D2, D1);
  _displayer->init();
  _displayer->flipScreenVertically();
  _displayer->setFont(ArialMT_Plain_10);
}
void Displayer::drarwTitle(String title) {
  _displayer->setFont(ArialMT_Plain_16);
  _displayer->drawString(5, 0, title);
}
void Displayer::displayWeather() {
  if (isWeatherDataReceived) {
    _displayer->setFont(ArialMT_Plain_10);
    this->drarwTitle("Pocket Weather");
    char t1[6];
    String t(_temperature,2);
    strcpy(t1, t.c_str());

    char h1[7];
    String h(_humidity,1);
    h.concat("%");
    strcpy(h1, h.c_str());
    this->drawLineOne(t1, h1);
    this->drawLineTwo(_pressure);
    isWeatherDataReceived = false;
  }
}
void Displayer::setWeatherData(double temperature, double humidity, double pressure) {
  _temperature = temperature;
  _humidity = humidity;
  dtostrf(pressure, 7, 2, _pressure);
  isWeatherDataReceived = true;
}
void Displayer::drawTextImage(const uint8_t *bitmap, uint8_t left, uint8_t top) {
  _displayer->drawXbm(left, top, ICON16_GLCD_WIDTH, ICON16_GLCD_HEIGHT, bitmap);
}
void Displayer::drawLineOne(const char *temperature, const char *humidity) {
  this->drawTextImage(temperatureIcon, 0, 21);

  _displayer->drawString(16, 21, temperature);
  _displayer->drawString(16 * 5 - 10, 21, humidity);
}
void Displayer::drawLineTwo(char *pressure) {
  this->drawTextImage(pressureIcon, 0, 39);

  _displayer->drawString(16, 39, pressure);
  // if (_isLaugh) {
  //   this->drawTextImage(faceLaugh, 16 * 5 - 3, 39);
  // } else {
  //   this->drawTextImage(faceNormal, 16 * 5 - 6, 39);
  // }
}
void Displayer::setLaugh(bool isLaugh) {
  _isLaugh = isLaugh;
}
void Displayer::loop() {
  _displayer->clear();
  displayWeather();
  _displayer->display();
}
