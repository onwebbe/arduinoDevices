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
#include "MenuItem.h"
#include <LinkedList.h>

// Optionally include custom images
#include "images.h"

SSD1306Wire display(0x3c, D1, D2);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h

#define DEMO_DURATION 3000

int currenLayer = 1;
String currentLayerText = "Main Menu";
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

//String menuItemList.get(] = {"Menu 1","Menu 2","Menu 3","Menu 4","Menu 5"};


LinkedList<SuperMenu_MenuItem*> mainMenuList = LinkedList<SuperMenu_MenuItem*>();

LinkedList<SuperMenu_MenuItem*> menuItemList = LinkedList<SuperMenu_MenuItem*>();


void setupMenu() {
  SuperMenu_MenuItem* menu1 = new SuperMenu_MenuItem("Menu1", 1);
  SuperMenu_MenuItem* menu11 = new SuperMenu_MenuItem("Menu1-1", 2);
  SuperMenu_MenuItem* menu12 = new SuperMenu_MenuItem("Menu1-2", 2);
  SuperMenu_MenuItem* menu13 = new SuperMenu_MenuItem("Menu1-3", 2);
  SuperMenu_MenuItem* menu14 = new SuperMenu_MenuItem("Menu1-4", 2);
  SuperMenu_MenuItem* menu15 = new SuperMenu_MenuItem("Menu1-5", 2);
  menu1->addSubMenu(menu11);
  menu1->addSubMenu(menu12);
  menu1->addSubMenu(menu13);
  menu1->addSubMenu(menu14);
  menu1->addSubMenu(menu15);
  
  SuperMenu_MenuItem* menu2 = new SuperMenu_MenuItem("Menu2", 1);
  SuperMenu_MenuItem* menu21 = new SuperMenu_MenuItem("Menu2-1", 2);
  SuperMenu_MenuItem* menu22 = new SuperMenu_MenuItem("Menu2-2", 2);
  SuperMenu_MenuItem* menu23 = new SuperMenu_MenuItem("Menu2-3", 2);
  SuperMenu_MenuItem* menu24 = new SuperMenu_MenuItem("Menu2-4", 2);
  SuperMenu_MenuItem* menu25 = new SuperMenu_MenuItem("Menu2-5", 2);
  menu2->addSubMenu(menu21);
  menu2->addSubMenu(menu22);
  menu2->addSubMenu(menu23);
  menu2->addSubMenu(menu24);
  menu2->addSubMenu(menu25);
  
  SuperMenu_MenuItem* menu3 = new SuperMenu_MenuItem("Menu3", 1);
  SuperMenu_MenuItem* menu4 = new SuperMenu_MenuItem("Menu4", 1);
  SuperMenu_MenuItem* menu5 = new SuperMenu_MenuItem("Menu5", 1);
  SuperMenu_MenuItem* menu6 = new SuperMenu_MenuItem("Menu6", 1);
  SuperMenu_MenuItem* menu7 = new SuperMenu_MenuItem("Menu7", 1);

  mainMenuList.add(menu1);
  mainMenuList.add(menu2);
  mainMenuList.add(menu3);
  mainMenuList.add(menu4);
  mainMenuList.add(menu5);
  mainMenuList.add(menu6);
  mainMenuList.add(menu7);
}
void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  Serial.println();
  randomSeed(analogRead(A0));

  // Initialising the UI will init the display too.
  display.init();

//  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  setupMenu();
  menuItemList = mainMenuList;
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
  if (moveStep < 0) {
    
    if (textStartPosition - 1 >= 0) {
      SuperMenu_MenuItem *menu = menuItemList.get(textStartPosition - 1);
      line1 = menu->getMenuText();
    }
    SuperMenu_MenuItem *menu = menuItemList.get(textStartPosition);
    line2 = menu->getMenuText();
    if (textStartPosition + 1 < menuItemList.size()) {
      SuperMenu_MenuItem *menu = menuItemList.get(textStartPosition + 1);
      line3 = menu->getMenuText();
    }
    if (textStartPosition + 2 < menuItemList.size()) {
      SuperMenu_MenuItem *menu = menuItemList.get(textStartPosition + 2);
      line4 = menu->getMenuText();
    }

    
  } else if (moveStep > 0) {
    line1Position = currentTextMoveY - rectHeight;
    line2Position = currentTextMoveY;
    line3Position = currentTextMoveY + rectHeight * 1;
    line4Position = currentTextMoveY + rectHeight * 2;
    
    line1 = menuItemList.get(textStartPosition)->getMenuText();
    if (textStartPosition + 1 < menuItemList.size()) {
      SuperMenu_MenuItem *menu = menuItemList.get(textStartPosition + 1);
      line2 = menu->getMenuText();
    }
    if (textStartPosition + 2 < menuItemList.size()) {
      SuperMenu_MenuItem *menu = menuItemList.get(textStartPosition + 2);
      line3 = menu->getMenuText();
    }
    if (textStartPosition + 3 < menuItemList.size()) {
      SuperMenu_MenuItem *menu = menuItemList.get(textStartPosition + 3);
      line4 = menu->getMenuText();
    }
  } else {
    SuperMenu_MenuItem *menu = menuItemList.get(textStartPosition);
    line1 = menu->getMenuText();
    if (textStartPosition + 1 < menuItemList.size()) {
      SuperMenu_MenuItem *menu = menuItemList.get(textStartPosition + 1);
      line2 = menu->getMenuText();
    }
    if (textStartPosition + 2 < menuItemList.size()) {
      SuperMenu_MenuItem *menu = menuItemList.get(textStartPosition + 2);
      line3 = menu->getMenuText();
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
