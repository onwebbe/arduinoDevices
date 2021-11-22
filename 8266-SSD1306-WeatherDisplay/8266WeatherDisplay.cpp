#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"

#include "8266WeatherDisplay.h"
SSD1306Wire _display(0x3c, SDA, SCL);
/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  32
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
#define ICON16_GLCD_HEIGHT 16
#define ICON16_GLCD_WIDTH  16

WeatherDisplay::WeatherDisplay() {
  
}
void WeatherDisplay::init() {
  _display.init();
  
  _display.flipScreenVertically();
  _display.setFont(ArialMT_Plain_10);
  
  delay(2000);
}
void WeatherDisplay::displayData(char *temperature, char *humidity, char *light, char *wind, char *pressure, char *rain) {
  _display.clear();
  this->drawLineOne(temperature, humidity);
  this->drawLineTwo(light, wind);
  this->drawLineThree(pressure, rain);
  _display.display();
}
void WeatherDisplay::drawTextImage(const uint8_t *bitmap, uint8_t left, uint8_t top) {
  _display.drawXbm(left, top, ICON16_GLCD_WIDTH, ICON16_GLCD_HEIGHT, bitmap);
}
void WeatherDisplay::drawLineOne(char *temperature, char *humidity) {
  this->drawTextImage(temperatureIcon, 0, 25);
  _display.setFont(ArialMT_Plain_16);

  float floatTemperature = atof(temperature);
//  Serial.println(floatTemperature);
  temperature = dtostrf(floatTemperature,5,1,temperature);
//  Serial.println(temperature);
//  String temperatureStr = "";
//  temperatureStr.concat(temperature);
//  char b[5];
//  temperatureStr = temperatureStr.substring(0, 5);
//  strcpy(b,temperatureStr.c_str());
//  b[4] = ' ';
  _display.drawString(16, 25, temperature);

  String humidityStr = "";
  humidityStr.concat(humidity);
  humidityStr.concat("%");
  char c[4];
  strcpy(c,humidityStr.c_str());
  _display.drawString(16 * 5, 25, c);
}
void WeatherDisplay::drawLineTwo(char *light, char *wind) {
  this->drawTextImage(lightIcon, 0, 48);
  _display.setFont(ArialMT_Plain_16);
  
  String lightStr = "";
  lightStr.concat(light);
  char b[5];
  lightStr = lightStr.substring(0, strlen(light) > 5 ? 5 : strlen(light));
  strcpy(b,lightStr.c_str());
  _display.drawString(16, 48, b);

  String windStr = "";
  float floatWind = atof(wind);
  wind = dtostrf(floatWind,5,1,wind);
//  windStr.concat(wind);
//  char c[4];
//  strcpy(c,windStr.c_str());
  _display.drawString(16 * 5, 48, wind);
}
void WeatherDisplay::drawLineThree(char *pressure, char *rain) {
  this->drawTextImage(pressureIcon, 0, 0);
  _display.setFont(ArialMT_Plain_16);
  
  String pressureStr = "";
  pressureStr.concat(pressure);
  char b[5];
  pressureStr = pressureStr.substring(0, strlen(pressure) > 5 ? 5 : strlen(pressure));
  strcpy(b,pressureStr.c_str());
  _display.drawString(16, 0, b);

  if (strcmp(rain, "true") == 0) {
    this->drawTextImage(rainIcon, 16 * 5, 0);
  } else {
    this->drawTextImage(sunIcon, 16 * 5, 0);
  }
  
//  if (rain == 0) {
//    this->drawTextImage(sunIcon, 16 * 5, 0);
//  } else {
//    this->drawTextImage(rainIcon, 16 * 5, 0);
//  }
}
void WeatherDisplay::displayConnecting() {
  _display.clear();
  _display.drawString(10, 0, "Connecting to WIFI...");
  int progress = (50) % 100;
  // draw the progress bar
  _display.drawProgressBar(0, 32, 120, 10, progress);
//
//  // draw the percentage as String
//  _display.setTextAlignment(TEXT_ALIGN_CENTER);
//  _display.drawString(64, 15, String(progress) + "%");
  _display.display();
}

void WeatherDisplay::displayWifiInfo() {
  _display.clear();
  _display.drawString(10, 0, "Ya!  We got Connected");
  _display.display();
}
