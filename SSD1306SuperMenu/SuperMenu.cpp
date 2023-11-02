#include "SuperMenu.h"
#include <LinkedList.h>
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"

//SSD1306Wire display(0x3c, D1, D2);
LinkedList<SuperMenu_MenuItem*> *menuItemList;

SuperMenu_Menu::SuperMenu_Menu(SSD1306Wire *displayer) {
  Serial.println("SuperMenu_Menu::SuperMenu_Menu");
//  SSD1306Wire *display = new SSD1306Wire(0x3c, D1, D2);
    this->display = displayer;
//  display->init();
////  this->display->flipScreenVertically();
//  display->setFont(ArialMT_Plain_10);
}
void SuperMenu_Menu::init() {
   Serial.println("SuperMenu_Menu::init");
   display->init();
   display->flipScreenVertically();
   display->setFont(ArialMT_Plain_10);
   menuItemList = mainMenuList;
}
void SuperMenu_Menu::addToMainMenu(SuperMenu_MenuItem* menuItem) {
  mainMenuList->add(menuItem);
}
int SuperMenu_Menu::getCurrentSelectedItem() {
  return currentSelectedItem;
}
String SuperMenu_Menu::getMenuFlow() {
  return "";
}

void SuperMenu_Menu::drawMenu() {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->normalDisplay();
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
      SuperMenu_MenuItem *menu = menuItemList->get(textStartPosition - 1);
      line1 = menu->getMenuText();
    }
    SuperMenu_MenuItem *menu = menuItemList->get(textStartPosition);
    line2 = menu->getMenuText();
    if (textStartPosition + 1 < menuItemList->size()) {
      SuperMenu_MenuItem *menu = menuItemList->get(textStartPosition + 1);
      line3 = menu->getMenuText();
    }
    if (textStartPosition + 2 < menuItemList->size()) {
      SuperMenu_MenuItem *menu = menuItemList->get(textStartPosition + 2);
      line4 = menu->getMenuText();
    }
  }  else if (moveStep > 0) {
    line1Position = currentTextMoveY - rectHeight;
    line2Position = currentTextMoveY;
    line3Position = currentTextMoveY + rectHeight * 1;
    line4Position = currentTextMoveY + rectHeight * 2;
    
    line1 = menuItemList->get(textStartPosition)->getMenuText();
    if (textStartPosition + 1 < menuItemList->size()) {
      SuperMenu_MenuItem *menu = menuItemList->get(textStartPosition + 1);
      line2 = menu->getMenuText();
    }
    if (textStartPosition + 2 < menuItemList->size()) {
      SuperMenu_MenuItem *menu = menuItemList->get(textStartPosition + 2);
      line3 = menu->getMenuText();
    }
    if (textStartPosition + 3 < menuItemList->size()) {
      SuperMenu_MenuItem *menu = menuItemList->get(textStartPosition + 3);
      line4 = menu->getMenuText();
    }
  }
  else {
    SuperMenu_MenuItem *menu = menuItemList->get(textStartPosition);
    line1 = menu->getMenuText();
    if (textStartPosition + 1 < menuItemList->size()) {
      SuperMenu_MenuItem *menu = menuItemList->get(textStartPosition + 1);
      line2 = menu->getMenuText();
    }
    if (textStartPosition + 2 < menuItemList->size()) {
      SuperMenu_MenuItem *menu = menuItemList->get(textStartPosition + 2);
      line3 = menu->getMenuText();
    }
  }

  display->setColor(WHITE);

  display->drawString(5, line1Position, line1);
  display->drawString(5, line2Position, line2);
  display->drawString(5, line3Position, line3);
  display->drawString(5, line4Position, line4);
  
  display->setColor(BLACK);
  display->fillRect(-1, -1, 129, 16);
  display->setColor(WHITE);
  display->drawString(5, 0, currentLayerText);
}
void SuperMenu_Menu::drawOutline() {
  int targetRectMoveY = currentMenuPosition * rectHeight;
  if (currentRectMoveY != targetRectMoveY) {
    if (targetRectMoveY > currentRectMoveY) {
      currentRectMoveY = currentRectMoveY + 1;
    } else {
      currentRectMoveY = currentRectMoveY - 1;
    }
  }
  display->setColor(WHITE);
  display->drawRect(0, currentRectMoveY + 1, 127, rectHeight + 1);
}
void SuperMenu_Menu::moveUp() {
    if (currentSelectedItem <= 1) {
      return;
    }
    prevSelectedItem = currentSelectedItem;
    currentSelectedItem = currentSelectedItem - 1;
    
    
    if (currentMenuPosition == 1 && currentSelectedItem < prevSelectedItem) {
      //move up
      Serial.print("moveup");
      moveStep = prevSelectedItem - currentSelectedItem;
      currentTextMoveY = rectHeight;
    } else {
      Serial.print("moveup-nomoave");
      moveStep = 0;
      currentTextMoveY = rectHeight;
    }

    if (currentSelectedItem > prevSelectedItem && currentMenuPosition < 3) {
      currentMenuPosition = currentMenuPosition + 1;
    } else if (currentSelectedItem < prevSelectedItem && currentMenuPosition > 1) {
      currentMenuPosition = currentMenuPosition - 1;
    }
    Serial.print("CurrentSelectedItem:");
    Serial.println(currentSelectedItem);
    Serial.print("currentMenuPosition:");
    Serial.println(currentMenuPosition);
}
void SuperMenu_Menu::moveDown() {
    if (currentSelectedItem >= menuItemList->size()) {
      return;
    }
    prevSelectedItem = currentSelectedItem;
    currentSelectedItem = currentSelectedItem + 1;
    if (currentMenuPosition == 3 && currentSelectedItem > prevSelectedItem && currentSelectedItem > 3) {
      //move down
      Serial.print("movedown");
      moveStep = prevSelectedItem - currentSelectedItem;
      currentTextMoveY = rectHeight;
    } else {
      Serial.print("movedown-nomoave");
      moveStep = 0;
      currentTextMoveY = rectHeight;
    }

    if (currentSelectedItem > prevSelectedItem && currentMenuPosition < 3) {
      currentMenuPosition = currentMenuPosition + 1;
    } else if (currentSelectedItem < prevSelectedItem && currentMenuPosition > 1) {
      currentMenuPosition = currentMenuPosition - 1;
    }
    Serial.print("CurrentSelectedItem:");
    Serial.println(currentSelectedItem);
    Serial.print("currentMenuPosition:");
    Serial.println(currentMenuPosition);
}
void SuperMenu_Menu::goNextLayer() {
  Serial.print("start next layer================");
  Serial.println(currentSelectedItem);
  SuperMenu_MenuItem *current = menuItemList->get(currentSelectedItem - 1);
  LinkedList<SuperMenu_MenuItem*>* subMenuList = current->getSubMenuList();
  if (subMenuList->size() > 0) {
    layeredMenu->add(menuItemList);
    layeredCurrentSelectedItem->add(currentSelectedItem);
    layeredCurrentMenuPosition->add(currentMenuPosition);

    
    currentSelectedItem = 1;
    currentMenuPosition = 1;
    prevSelectedItem = 1;
    moveStep = 0;
    currentTextMoveY = rectHeight;
    menuItemList = subMenuList;
  }

}
void SuperMenu_Menu::goBackLayer() {
  Serial.print("start back layer================");
  Serial.println(currentSelectedItem);
  if (layeredMenu->size() > 0) {
    menuItemList = layeredMenu->pop();
    currentSelectedItem = layeredCurrentSelectedItem->pop();
    currentMenuPosition = layeredCurrentMenuPosition->pop();
    
    prevSelectedItem = currentSelectedItem;
    moveStep = 0;
    currentTextMoveY = rectHeight;
  }
}
void SuperMenu_Menu::loop() {
  display->clear();
  drawMenu();
  drawOutline();
  display->display();
}

int SuperMenu_Menu::getCurrentSelectedMenu() {
  return currentSelectedItem;
}
