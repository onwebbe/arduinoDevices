/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 by ThingPulse, Daniel Eichhorn
 * Copyright (c) 2018 by Fabrice Weinberg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * ThingPulse invests considerable time and money to develop these open source libraries.
 * Please support us by buying our products (and not the clones) from
 * https://thingpulse.com
 *
 */
 
// Include the correct display library

// For a connection via I2C using the Arduino Wire include:
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"

// Optionally include custom images
#include "images.h"

SSD1306Wire display(0x3c, D1, D2);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h

#define DEMO_DURATION 3000

int currenLayer = 1;
int currentPosition = 1;


int rectHeight = 15;

// current selected menu number like if you have 5 menus and selected the 4th, the value is 4
int currentSelectedItem = 1;
int prevSelectedItem = 1;

// menu is 1 to 3 means highlight the first menu, second menu and third menu
int currentMenuPosition = 1;
int currentRectMoveY =  currentMenuPosition * rectHeight;

// the position of the first line of text
int currentTextPosition = 1;
int currentTextMoveY = rectHeight;
int moveStep = 0;

int sleepTime = 2500;

String menuItems[] = {"Menu 1","Menu 2","Menu 3","Menu 4","Menu 5"};

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  randomSeed(analogRead(A0));

  // Initialising the UI will init the display too.
  display.init();

//  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

}

void drawMenu() {
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.normalDisplay();
  int targetTextMoveY = 15 + (moveStep * rectHeight);
  if (moveStep < 0 && currentTextMoveY >= targetTextMoveY) {
    //move down
    currentTextMoveY = currentTextMoveY - 1;
  } else if (moveStep > 0 && currentTextMoveY <= targetTextMoveY) {
    // move up
    currentTextMoveY = currentTextMoveY + 1;
  }
  
  String line1 = "";
  String line2 = "";
  String line3 = "";
  String line4 = "";
  int line1Position = currentTextMoveY;
  int line2Position = currentTextMoveY + rectHeight;
  int line3Position = currentTextMoveY + rectHeight * 2;
  int line4Position = currentTextMoveY + rectHeight * 3;

  int textStartPosition = currentSelectedItem - currentMenuPosition;
  int textEndPosition = textStartPosition + 2;
//  if (currentSelectedItem > 0 && currentSelectedItem <=3) {
//    line1 = menuItems[0];
//    line2 = menuItems[1];
//    line3 = menuItems[2];
//  } else if (currentSelectedItem > (sizeof(menuItems) - 3) && currentSelectedItem <= sizeof(menuItems)) {
//    line1 = menuItems[sizeof(menuItems) - 3];
//    line2 = menuItems[sizeof(menuItems) - 2];
//    line3 = menuItems[sizeof(menuItems) - 1];
//  } else 
  if (moveStep < 0) {
//    line1Position = currentTextMoveY - rectHeight;
//    line2Position = currentTextMoveY;
//    line3Position = currentTextMoveY + rectHeight * 1;
//    line4Position = currentTextMoveY + rectHeight * 2;
    
    if (textStartPosition - 1 >= 0) {
      line1 = menuItems[textStartPosition - 1];
    }
    line2 = menuItems[textStartPosition];
    if (textStartPosition + 1 < sizeof(menuItems)) {
      line3 = menuItems[textStartPosition + 1];
    }
    if (textStartPosition + 2 < sizeof(menuItems)) {
      line4 = menuItems[textStartPosition + 2];
    }

    
  } else if (moveStep > 0) {
    line1Position = currentTextMoveY - rectHeight;
    line2Position = currentTextMoveY;
    line3Position = currentTextMoveY + rectHeight * 1;
    line4Position = currentTextMoveY + rectHeight * 2;
    
    line1 = menuItems[textStartPosition];
    if (textStartPosition + 1 < sizeof(menuItems)) {
      line2 = menuItems[textStartPosition + 1];
    }
    if (textStartPosition + 2 < sizeof(menuItems)) {
      line3 = menuItems[textStartPosition + 2];
    }
    if (textStartPosition + 3 < sizeof(menuItems)) {
      line4 = menuItems[textStartPosition + 3];
    }
  } else {
    line1 = menuItems[textStartPosition];
    if (textStartPosition + 1 < sizeof(menuItems)) {
      line2 = menuItems[textStartPosition + 1];
    }
    if (textStartPosition + 2 < sizeof(menuItems)) {
      line3 = menuItems[textStartPosition + 2];
    }
  }
  display.setColor(WHITE);

  display.drawString(5, line1Position, line1);
  display.drawString(5, line2Position, line2);
  display.drawString(5, line3Position, line3);
  display.drawString(5, line4Position, line4);
  
  display.setColor(BLACK);
  display.fillRect(-1, -1, 129, 16);
  display.setColor(WHITE);
  display.drawString(5, 0, "Hello world");
}
void drawOutline() {
  int targetRectMoveY = currentMenuPosition * rectHeight;
  if (currentRectMoveY != targetRectMoveY) {
    if (targetRectMoveY > currentRectMoveY) {
      currentRectMoveY = currentRectMoveY + 1;
    } else {
      currentRectMoveY = currentRectMoveY - 1;
    }
  }
  display.setColor(WHITE);
  display.drawRect(0, currentRectMoveY + 1, 127, rectHeight + 1);
}
unsigned long compareValue = sleepTime;
boolean down = true;
void loop() {
  // clear the display
  int count = 0;
  unsigned long currentTime = millis();
  if (currentTime > compareValue) {
    compareValue = compareValue + sleepTime;
    
    double randomValue = random(0,10);
    Serial.println(randomValue);
    if (randomValue >= 5) {
      down = true;
    } else {
      down = false;
    }
    if (currentSelectedItem == 5) {
      down = true;
    }
    if (currentSelectedItem == 1) {
      down = false;
    }
    prevSelectedItem = currentSelectedItem;
    if (down) {
      currentSelectedItem = currentSelectedItem - 1;
    } else {
      currentSelectedItem = currentSelectedItem + 1;
    }
    
    
    if (currentMenuPosition == 1 && currentSelectedItem < prevSelectedItem && currentSelectedItem < 3) {
      //move up
//      Serial.print("moveup");
      moveStep = prevSelectedItem - currentSelectedItem;
      currentTextMoveY = rectHeight;
    } else if (currentMenuPosition == 3 && currentSelectedItem > prevSelectedItem && currentSelectedItem > 3) {
      //move down
//      Serial.print("movedown");
      moveStep = prevSelectedItem - currentSelectedItem;
      currentTextMoveY = rectHeight;
    } else {
//      Serial.print("nomove");
      moveStep = 0;
      currentTextMoveY = rectHeight;
    }

    if (currentSelectedItem > prevSelectedItem && currentMenuPosition < 3) {
      currentMenuPosition = currentMenuPosition + 1;
    } else if (currentSelectedItem < prevSelectedItem && currentMenuPosition > 1) {
      currentMenuPosition = currentMenuPosition - 1;
    }
//    Serial.print(prevSelectedItem);
//    Serial.print("-");
//    Serial.print(currentSelectedItem);
//    Serial.print("-");
//    Serial.print(currentMenuPosition);
//    Serial.print("-");
//    Serial.print(currentTextMoveY);
//    Serial.print("-");
//    Serial.println(moveStep);
    
  }
  display.clear();
  drawMenu();
  drawOutline();
  display.display();
  delay(10);
}
