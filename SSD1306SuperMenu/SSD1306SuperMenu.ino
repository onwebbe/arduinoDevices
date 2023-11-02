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
#include "SuperMenu.h"
#include <OneButton.h>


// Optionally include custom images
#include "images.h"

#define UP_PIN 16
#define DOWN_PIN 14
#define NEXT_PIN 13
#define PREV_PIN 12
#define CONF_PIN 15

// SSD1306Wire display(0x3c, D1, D2);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h
SSD1306Wire *display = new SSD1306Wire(0x3c, D1, D2);

#define DEMO_DURATION 3000
OneButton up(UP_PIN, true);
OneButton down(DOWN_PIN, true);
OneButton next(NEXT_PIN, true);
OneButton prev(PREV_PIN, true);
OneButton conf(CONF_PIN, true);
//int currenLayer = 1;
//String currentLayerText = "Main Menu";
//int currentPosition = 1;


int sleepTime = 2500;

SuperMenu_Menu menu = SuperMenu_Menu(display);

ICACHE_RAM_ATTR void moveupTicks() {
  // include all buttons here to be checked
  Serial.println("Up terminate");
  //clickUp();
  up.tick(); // just call tick() to check the state.
}

ICACHE_RAM_ATTR void movedownTicks() {
  Serial.println("down terminate");
  // clickDown();
  // include all buttons here to be checked
  down.tick(); // just call tick() to check the state.
}

ICACHE_RAM_ATTR void moveNextTicks() {
  // include all buttons here to be checked
  Serial.println("next terminate");
  //clickNext();
  next.tick(); // just call tick() to check the state.
}

ICACHE_RAM_ATTR void movePrevTicks() {
  Serial.println("prev terminate");
  //clickPrev();
  // include all buttons here to be checked
  prev.tick(); // just call tick() to check the state.
}
ICACHE_RAM_ATTR void moveConfTicks() {
  Serial.println("confirm terminate");
  //clickPrev();
  // include all buttons here to be checked
  conf.tick(); // just call tick() to check the state.
}

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

  menu.addToMainMenu(menu1);
  menu.addToMainMenu(menu2);
  menu.addToMainMenu(menu3);
  menu.addToMainMenu(menu4);
  menu.addToMainMenu(menu5);
  menu.addToMainMenu(menu6);
  menu.addToMainMenu(menu7);
}
void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  Serial.println();
  
  attachInterrupt(digitalPinToInterrupt(UP_PIN), moveupTicks, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DOWN_PIN), movedownTicks, CHANGE);
  attachInterrupt(digitalPinToInterrupt(NEXT_PIN), moveNextTicks, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PREV_PIN), movePrevTicks, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CONF_PIN), moveConfTicks, CHANGE);
  
  up.attachClick(clickUp);
  down.attachClick(clickDown);
  next.attachClick(clickNext);
  prev.attachClick(clickPrev);
  conf.attachClick(clickConf);
  setupMenu();
  menu.init();
}

unsigned long compareValue = sleepTime;
void clickUp() {
  Serial.println("clickup");
  menu.moveUp();
}
void clickDown() {
  Serial.println("movedownTick");
  menu.moveDown();
}
void clickNext() {
  Serial.println("movebextTick");
  menu.goNextLayer();
}
void clickPrev() {
  Serial.println("moveprevTick");
  menu.goBackLayer();
}
void clickConf() {
  Serial.println("confirmTick");
}
void loop() {
  // clear the display
//  boolean down = false;
//  int count = 0;
//  unsigned long currentTime = millis();
//  if (currentTime > compareValue) {
//    compareValue = compareValue + sleepTime;
//    
//    double randomValue = random(0,10);
//    Serial.println(randomValue);
//    if (randomValue >= 5) {
//      down = true;
//    } else {
//      down = false;
//    }
//    int currentSelectedItem = menu.getCurrentSelectedMenu();
//    if (currentSelectedItem == 5) {
//      down = true;
//    }
//    if (currentSelectedItem == 1) {
//      down = false;
//    }
//    if (down) {
//      menu.moveDown();
//    } else {
//      menu.moveUp();
//    }
//  }
  menu.loop();
  up.tick();
  down.tick();
  prev.tick();
  next.tick();
  conf.tick();
//  Serial.println("start loop");
  
}
