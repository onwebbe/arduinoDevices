// Include the correct display library
 // For a connection via I2C using Wire include
 #include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
 #include <ESP8266WiFi.h>
 #include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
// Include the UI lib
#include "OLEDDisplayUi.h"
#include "images.h"
#include "OneButton.h"
// Initialize the OLED display using Wire library
// SDA, SCL
SSD1306Wire  display(0x3c, SDA, SCL);
// SH1106Wire display(0x3c, D3, D5);

OLEDDisplayUi ui     ( &display );

#define NEXT_BUTTON D5
#define PREV_BUTTON D6
#define UP_BUTTON D7
#define DOWN_BUTTON D8

String menuInfoFun[] = {menuitem1, menuitem2, menuitem3};
int currentMenuPosition = 0;

OneButton nextButton(NEXT_BUTTON, true);
OneButton prevButton(PREV_BUTTON, true);

void menuitem1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  // draw an xbm image.
  // Please note that everything that should be transitioned
  // needs to be drawn relative to x and y

  display->drawXbm(x + 34, y + 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}
void menuitem2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  // draw an xbm image.
  // Please note that everything that should be transitioned
  // needs to be drawn relative to x and y

  display->drawXbm(x + 34, y + 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}
void menuitem3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  // draw an xbm image.
  // Please note that everything that should be transitioned
  // needs to be drawn relative to x and y

  display->drawXbm(x + 34, y + 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  // draw an xbm image.
  // Please note that everything that should be transitioned
  // needs to be drawn relative to x and y

  display->drawXbm(x + 34, y + 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

void drawFrame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  // Demonstrates the 3 included default sizes. The fonts come from SSD1306Fonts.h file
  // Besides the default fonts there will be a program to convert TrueType fonts into this format
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(0 + x, 10 + y, "Arial 10");

  display->setFont(ArialMT_Plain_16);
  display->drawString(0 + x, 20 + y, "Arial 16");

  display->setFont(ArialMT_Plain_24);
  display->drawString(0 + x, 34 + y, "Arial 24");
}
FrameCallback frames[] = { drawFrame1, drawFrame2 };
int frameCount = 2;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Initialize Buttons
  pinMode(NEXT_BUTTON, INPUT);
  pinMode(PREV_BUTTON, INPUT);
  pinMode(UP_BUTTON, INPUT);
  pinMode(DOWN_BUTTON, INPUT);

  
  ui.setTargetFPS(60);

  // Customize the active and inactive symbol
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);
  ui.setIndicatorPosition(BOTTOM);

  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);
  ui.disableAutoTransition();

  // Add frames
  ui.setFrames(frames, frameCount);
  
  ui.init();

  display.flipScreenVertically();


  nextButton.attachClick(nextCicked);
  prevButton.attachClick(prevCicked);

  // Set WiFi to station mode
  WiFi.mode(WIFI_STA);

  // Disconnect from an AP if it was previously connected
  WiFi.disconnect();
  delay(100);
  int scanResult;
  scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/false);
  String ssid;
  int32_t rssi;
  uint8_t encryptionType;
  uint8_t* bssid;
  int32_t channel;
  bool hidden;
  String *ssidList;
  int32_t *rssiList;
  if (scanResult == 0) {
    Serial.println(F("No networks found"));
  } else if (scanResult > 0) {
    Serial.printf(PSTR("%d networks found:\n"), scanResult);
    String ssidListTmp[scanResult];
    ssidList = ssidListTmp;
    int32_t rssiListTmp[scanResult];
    rssiList = rssiListTmp;
    // Print unsorted scan results
    for (int8_t i = 0; i < scanResult; i++) {
      WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);
      ssidListTmp[i] = ssid.c_str();
      rssiListTmp[i] = rssi;
      delay(0);
    }
  } else {
    Serial.printf(PSTR("WiFi scan error %d"), scanResult);
  }
}
void nextCicked() {
  Serial.println("next button clicked");
  ui.setIndicatorDirection(LEFT_RIGHT);
  ui.nextFrame();
}
void prevCicked() {
  Serial.println("prev button clicked");
  ui.setIndicatorDirection(RIGHT_LEFT);
  ui.previousFrame();
}
void loop() {
  // put your main code here, to run repeatedly:
  int remainingTimeBudget = ui.update();
  nextButton.tick();
  prevButton.tick();
  if (remainingTimeBudget > 0) {
    // You can do some work here
    // Don't do stuff if you are below your
  
    delay(remainingTimeBudget);
  }
}
