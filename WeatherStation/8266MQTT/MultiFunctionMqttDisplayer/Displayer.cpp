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
const uint8_t activeSymbol[] PROGMEM = {
    B00011000,
    B00100100,
    B01000010,
    B01000010,
    B00100100,
    B00011000
};

const uint8_t inactiveSymbol[] PROGMEM = {
    B00000000,
    B00000000,
    B00011000,
    B00011000,
    B00000000,
    B00000000
};

static const unsigned char PROGMEM lighterIcon[] =
{ B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B10000000, B00000011,
  B00110000, B00011000,
  B10011000, B00110011,
  B10011000, B00110001,
  B00110000, B00011011,
  B10000000, B00000011,
  B10000000, B00000011,
  B11100000, B00001111,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000
};

static const unsigned char PROGMEM pumbIcon[] =
{ B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B11100000, B00000111,
  B11111100, B00000111,
  B11111100, B00000111,
  B11111100, B01111111,
  B11100000, B01111111,
  B11100000, B01111111,
  B11100000, B00000111,
  B11100000, B00000111,
  B11100000, B00000111,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000
};
static const unsigned char PROGMEM heaterIcon[] =
{ B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00110000, B00110011,
  B10001000, B00001000,
  B00100000, B00100010,
  B10001000, B00001000,
  B00100000, B00100010,
  B10001000, B00001000,
  B0010000,  B00100010,
  B11111100, B00111111,
  B11111100, B00111111,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000
};
#define ICON16_GLCD_HEIGHT 16
#define ICON16_GLCD_WIDTH  16

Displayer::Displayer() {
  _hetunBright = 0;
  _hetunPumb = 0;
  _hetunHeater = false;
  _hetunTemperature = 0;

  _leilongBright = 0;
  _leilongPumb = 0;
  _leilongHeater = false;
  _leilongTemperature = 0;
}
void Displayer::init() {
  _displayer = new SSD1306Wire(0x3c, D2, D1);
  _displayer->init();
  _displayer->flipScreenVertically();
  _displayer->setFont(ArialMT_Plain_10);
}
void Displayer::drawPaginator() {
  for(int i = 0; i < 3; i++) {
    if (i == currentPage) {
      this->drawActiveInactiveIcon(activeSymbol, 49 + (i * 10), 58);
    } else {
      this->drawActiveInactiveIcon(inactiveSymbol, 49 + (i * 10), 58);
    }
  }
}
void Displayer::drarwTitle(String title, const char *rain, char *wind) {
  _displayer->setFont(ArialMT_Plain_16);
  _displayer->drawString(5, 0, title);
//  if (strcmp(rain, "true") == 0) {
//    this->drawTextImage(rainIcon, 70, 2);
//  } else {
//    this->drawTextImage(sunIcon, 70, 2);
//  }
  this->drawTextImage(sunIcon, 70, 2);
  _displayer->drawString(90, 0, wind);
}
void Displayer::displayWeather() {
  if (isWeatherDataReceived) {
    _displayer->setFont(ArialMT_Plain_10);
    this->drarwTitle("Weather", _isRain, _wind);
    char t1[6];
    String t(_temperature,2);
    strcpy(t1, t.c_str());

    char h1[7];
    String h(_humidity,1);
    h.concat("%");
    strcpy(h1, h.c_str());
    this->drawLineOne(t1, h1);
    this->drawLineTwo(_light, _pressure);
  }
}
void Displayer::setWeatherData(double temperature, double humidity, double light, double wind, double pressure, const char *rain) {
  _temperature = temperature;
  _humidity = humidity;
  
  dtostrf(light, -5, 0, _light);

  dtostrf(wind, -5, 1, _wind);

  dtostrf(pressure, -6, 1, _pressure);


  isWeatherDataReceived = true;
}
void Displayer::drawTextImage(const uint8_t *bitmap, uint8_t left, uint8_t top) {
  _displayer->drawXbm(left, top, ICON16_GLCD_WIDTH, ICON16_GLCD_HEIGHT, bitmap);
}
void Displayer::drawActiveInactiveIcon(const uint8_t *bitmap, uint8_t left, uint8_t top) {
  _displayer->drawXbm(left, top, 8, 6, bitmap);
}
void Displayer::drawLineOne(const char *temperature, const char *humidity) {
  this->drawTextImage(temperatureIcon, 0, 21);

//  float floatTemperature = atof(temperature);
//  Serial.println(floatTemperature);
//  temperature = dtostrf(floatTemperature,5,1,temperature);
//  Serial.println(temperature);
//  String temperatureStr = "";
//  temperatureStr.concat(temperature);
//  char b[5];
//  temperatureStr = temperatureStr.substring(0, 5);
//  strcpy(b,temperatureStr.c_str());
//  b[4] = ' ';
  _displayer->drawString(16, 21, temperature);

//  String humidityStr = "";
//  humidityStr.concat(humidity);
//  humidityStr.concat("%");
//  char c[6];
//  strcpy(c,humidityStr.c_str());
  _displayer->drawString(16 * 5 - 10, 21, humidity);
}
void Displayer::drawLineTwo(char *light, char *pressure) {
  this->drawTextImage(lightIcon, 0, 39);
//  
//  String lightStr = "";
//  lightStr.concat(light);
//  char b[5];
//  lightStr = lightStr.substring(0, strlen(light) > 5 ? 5 : strlen(light));
//  strcpy(b,lightStr.c_str());
  _displayer->drawString(16, 39, light);

//  String windStr = "";
//  float floatWind = atof(wind);
//  wind = dtostrf(floatWind,5,1,wind);
//  windStr.concat(wind);
//  char c[4];
//  strcpy(c,windStr.c_str());
  _displayer->drawString(16 * 5 - 10, 39, pressure);
}

void Displayer::displayConnecting() {
  _displayer->clear();
  _displayer->drawString(10, 0, "Connecting to WIFI...");
  int progress = (50) % 100;
  // draw the progress bar
  _displayer->drawProgressBar(0, 32, 120, 10, progress);
//
//  // draw the percentage as String
//  _displayer->setTextAlignment(TEXT_ALIGN_CENTER);
//  _displayer->drawString(64, 15, String(progress) + "%");
  _displayer->display();
}

void Displayer::displayWifiInfo() {
  _displayer->clear();
  _displayer->drawString(10, 0, "Ya!  We got Connected");
  
}

void Displayer::displayYugangHeTun() {
//  Serial.print("---_hetunHeater---");
//  Serial.print(_hetunHeater);
//  Serial.print("---_hetunBright---");
//  Serial.print(_hetunBright);
//  Serial.print("---_hetunPumb---");
//  Serial.print(_hetunPumb);
//  Serial.print("---_hetunTemperature---");
//  Serial.println(_hetunTemperature);
  
  _displayer->setFont(ArialMT_Plain_16);
  _displayer->drawString(5, 0, "HeTun Aquarium");

  this->drawTextImage(heaterIcon, 0, 21);
  _displayer->drawString(16, 21, _hetunHeater?"ON":"OFF");
  this->drawTextImage(lighterIcon, 16 * 4, 21);
  double brt = _hetunBright * 100 / 255;
  String br(brt,0);
  br.concat("%");
  _displayer->drawString(16 * 5, 21, br.c_str());

  this->drawTextImage(pumbIcon, 0, 39);
  double put = _hetunPumb * 100 / 255;
  String pu(put,0);
  pu.concat("%");
  
//  Serial.println("HeTun Bright");
//  Serial.println(pu);
  _displayer->drawString(16, 38, pu.c_str());
  this->drawTextImage(temperatureIcon, 16 * 4, 38);
  String te(_hetunTemperature, 0);
  _displayer->drawString(16 * 5, 38, te.c_str());
}
void Displayer::displayYugangLeilong() {
  _displayer->setFont(ArialMT_Plain_16);
  _displayer->drawString(5, 0, "LeiL Aquarium");

  this->drawTextImage(heaterIcon, 0, 21);
  _displayer->drawString(16, 21, _leilongHeater?"ON":"OFF");
  this->drawTextImage(lighterIcon, 16 * 4, 21);
  double brt = _leilongBright * 100 / 255;
  String br(brt,0);
  br.concat("%");
  _displayer->drawString(16 * 5, 21, br.c_str());

  this->drawTextImage(pumbIcon, 0, 39);
  double put = _leilongPumb * 100 / 255;
  String pu(put, 0);
  pu.concat("%");
//  Serial.println("HeTun Bright");
//  Serial.println(pu);
  _displayer->drawString(16, 38, pu.c_str());
  this->drawTextImage(temperatureIcon, 16 * 4, 38);
  String te(_leilongTemperature, 0);
  _displayer->drawString(16 * 5, 38, te.c_str());
}
void Displayer::setYugangHeTunLight(int light) {
  Serial.print("---light---");
  Serial.print(light);
  _hetunBright = light;
  Serial.print("---");
  Serial.println(_hetunBright);
}
void Displayer::setYugangHeTunPump(int pumb) {
  Serial.print("---pumb---");
  Serial.print(pumb);
  _hetunPumb = pumb;
  Serial.print("---");
  Serial.println(_hetunPumb);
}
void Displayer::setYugangHeTunHeater(bool heat) {
  Serial.print("---heat---");
  Serial.print(heat);
  _hetunHeater = heat;
  Serial.print("---");
  Serial.println(_hetunHeater);
}
void Displayer::setYugangHeTunTemperature(double temperature) {
  Serial.print("---temperature---");
  Serial.print(temperature);
  _hetunTemperature = temperature;
  Serial.print("---");
  Serial.println(_hetunTemperature);
}

void Displayer::setYugangLeiLongLight(int light) {
  _leilongBright = light;
}
void Displayer::setYugangLeiLongPump(int pumb) {
  _leilongPumb = pumb;
}
void Displayer::setYugangLeiLongHeater(bool heat) {
  _leilongHeater = heat;
}
void Displayer::setYugangLeiLongTemperature(double temperature) {
  _leilongTemperature = temperature;
}
void Displayer::nextPage() {
  currentPage = currentPage + 1;
  if (currentPage > 2) {
    currentPage = 0;
  }
}
void Displayer::prevPage() {
  currentPage = currentPage - 1;
  if (currentPage < 0) {
    currentPage = 0;
  }
}
void Displayer::loop() {
  _displayer->clear();
  if (currentPage == 0) {
    displayWeather();
  } else if (currentPage == 1) {
    displayYugangHeTun();
  } else {
    displayYugangLeilong();
  }
  drawPaginator();
  _displayer->display();
}
