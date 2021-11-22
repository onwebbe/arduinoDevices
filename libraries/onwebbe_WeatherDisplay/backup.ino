///*********************************************************************
//  This is an example for our Monochrome OLEDs based on SH110X drivers
//
//  This example is for a 128x64 size display using I2C to communicate
//  3 pins are required to interface (2 I2C and one reset)
//
//  Adafruit invests time and resources providing this open source code,
//  please support Adafruit and open-source hardware by purchasing
//  products from Adafruit!
//
//  Written by Limor Fried/Ladyada  for Adafruit Industries.
//  BSD license, check license.txt for more information
//  All text above, and the splash screen must be included in any redistribution
//
//  i2c SH1106 modified by Rupert Hirst  12/09/21
//*********************************************************************/
//
//
//
//#include <SPI.h>
//#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SH110X.h>
//
///* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
//#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
////#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's
//
//#define SCREEN_WIDTH 128 // OLED display width, in pixels
//#define SCREEN_HEIGHT 64 // OLED display height, in pixels
//#define OLED_RESET -1   //   QT-PY / XIAO
//Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//
//
//#define NUMFLAKES 10
//#define XPOS 0
//#define YPOS 1
//#define DELTAY 2
//
//
//#define LOGO16_GLCD_HEIGHT 16
//#define LOGO16_GLCD_WIDTH  16
//
//static const unsigned char PROGMEM temperatureIcon[] =
//{ B00000000, B00000000,
//  B00000111, B11000000,
//  B00000100, B01000000,
//  B00000111, B01000000,
//  B00000100, B01000000,
//  B00000111, B01000000,
//  B00000100, B01000000,
//  B00000111, B01000000,
//  B00000100, B01000000,
//  B00000100, B01000000,
//  B00000111, B11000000,
//  B00001111, B11100000,
//  B00001111, B11100000,
//  B00001111, B11100000,
//  B00000111, B11000000,
//  B00000001, B10000000
//};
//static const unsigned char PROGMEM lightIcon[] =
//{
//  B00000000, B00000000,
//  B00000000, B00000000,
//  B00011111, B11110000,
//  B00011110, B00010000,
//  B00011110, B00010000,
//  B00011110, B00010000,
//  B00011110, B00010000,
//  B00011110, B00010000,
//  B00011110, B00010000,
//  B00011110, B00010000,
//  B00011110, B00010000,
//  B00011110, B00010000,
//  B00011110, B00010000,
//  B00011111, B11110000,
//  B00000000, B00000000,
//  B00000000, B00000000
//};
//static const unsigned char PROGMEM pressureIcon[] =
//{ B00000000, B00000000,
//  B00000111, B11000000,
//  B00000111, B11000000,
//  B00000111, B11000000,
//  B00000111, B11000000,
//  B00000111, B11000000,
//  B00000111, B11000000,
//  B00000111, B11000000,
//  B00000111, B11000000,
//  B00011111, B11110000,
//  B00001111, B11100000,
//  B00000110, B11000000,
//  B00000001, B00000000,
//  B00111100, B01111000,
//  B00000011, B10000000,
//  B00000000, B00000000
//};
//static const unsigned char PROGMEM sunIcon[] =
//{
//  B00000001, B00000000,
//  B00000001, B00000000,
//  B00010001, B00010000,
//  B00001011, B10100000,
//  B00000111, B11100000,
//  B00001111, B11100000,
//  B01111111, B11111100,
//  B00001111, B11100000,
//  B00000111, B11000000,
//  B00001011, B10100000,
//  B00010001, B00010000,
//  B00000001, B00000000,
//  B00000001, B00000000,
//  B00000000, B00000000,
//  B00000000, B00000000,
//  B00000000, B00000000
//};
//static const unsigned char PROGMEM rainIcon[] =
//{
//  B00000000, B00000000,
//  B00000000, B00000000,
//  B00001000, B00000000,
//  B00011000, B00000000,
//  B00111100, B00000000,
//  B00011000, B00100000,
//  B00000000, B01100000,
//  B00000000, B11110000,
//  B00000000, B01100000,
//  B00000100, B00000000,
//  B00001100, B00000000,
//  B00011110, B00010000,
//  B00001100, B00110000,
//  B00000000, B01111000,
//  B00000000, B00110000,
//  B00000000, B00000000
//};
//#define ICON16_GLCD_HEIGHT 16
//#define ICON16_GLCD_WIDTH  16
//void setup()   {
//
//  Serial.begin(9600);
//
//  // Show image buffer on the display hardware.
//  // Since the buffer is intialized with an Adafruit splashscreen
//  // internally, this will display the splashscreen.
//
//  display.begin(i2c_Address, true); // Address 0x3C default
// //display.setContrast (0); // dim display
// 
//  display.display();
//  delay(2000);
//
//  // Clear the buffer.
//  display.clearDisplay();
//  // draw a bitmap icon and 'animate' movement
////  drawLineOne(-32.1, 12);
////  drawLineTwo(31001, 50);
////  drawLineThree(19521, 1);
////  display.display();
////  testdrawchar();
//  displayData(25.1, 80, 31001, 21, 12841, 0);
//}
//
//
//void loop() {
//
//}
//void displayData(float temperature, int humidity, int light, int wind, int pressure, int rain) {
//  drawLineOne(temperature, humidity);
//  drawLineTwo(light, wind);
//  drawLineThree(pressure, rain);
//  display.display();
//}
//void drawTextImage(const uint8_t *bitmap, uint8_t left, uint8_t top) {
//  display.drawBitmap(left, top, bitmap, ICON16_GLCD_WIDTH, ICON16_GLCD_HEIGHT, SH110X_WHITE);
//}
//void drawLineOne(float temperature, int humidity) {
//  drawTextImage(temperatureIcon, 0, 0);
//  display.setTextSize(2);
//  display.setTextColor(SH110X_WHITE);
//  display.setCursor(16, 0);
//  String temperatureStr = "";
//  temperatureStr.concat(temperature);
//  char b[5];
//  temperatureStr = temperatureStr.substring(0, 5);
//  strcpy(b,temperatureStr.c_str());
//  display.write(b);
//
//  String humidityStr = "";
//  humidityStr.concat(humidity);
//  humidityStr.concat("%");
//  char c[4];
//  strcpy(c,humidityStr.c_str());
//  display.setCursor(16 * 5, 0);
//  display.write(c);
//}
//void drawLineTwo(int light, int wind) {
//  drawTextImage(lightIcon, 0, 25);
//  display.setTextSize(2);
//  display.setTextColor(SH110X_WHITE);
//  display.setCursor(16, 25);
//  String lightStr = "";
//  lightStr.concat(light);
//  char b[5];
//  lightStr = lightStr.substring(0, 5);
//  strcpy(b,lightStr.c_str());
//  display.write(b);
//
//  String windStr = "";
//  windStr.concat(wind);
//  char c[4];
//  strcpy(c,windStr.c_str());
//  display.setCursor(16 * 5, 25);
//  display.write(c);
//}
//void drawLineThree(int pressure, int rain) {
//  drawTextImage(pressureIcon, 0, 48);
//  display.setTextSize(2);
//  display.setTextColor(SH110X_WHITE);
//  display.setCursor(16, 48);
//  String pressureStr = "";
//  pressureStr.concat(pressure);
//  char b[5];
//  pressureStr = pressureStr.substring(0, 5);
//  strcpy(b,pressureStr.c_str());
//  display.write(b);
//  
//  if (rain == 0) {
//    drawTextImage(sunIcon, 16 * 5, 48);
//  } else {
//    drawTextImage(rainIcon, 16 * 5, 48);
//  }
//}
