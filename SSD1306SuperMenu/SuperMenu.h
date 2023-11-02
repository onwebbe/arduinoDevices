#ifndef __SUPERMENU_MENU_H
#define __SUPERMENU_MENU_H
#include "MenuItem.h"
#include <LinkedList.h>
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"

class SuperMenu_Menu{
  private:
    SSD1306Wire *display;
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

    int currentColor = 255;
    
    // the position of the first line of text
    int currentTextPosition = 1;
    int currentTextMoveY = rectHeight;
    int moveStep = 0;

    LinkedList<SuperMenu_MenuItem*> *mainMenuList = new LinkedList<SuperMenu_MenuItem*>();
    
    LinkedList<SuperMenu_MenuItem*> *menuItemList;
    
    LinkedList<LinkedList<SuperMenu_MenuItem*>*> *layeredMenu = new LinkedList<LinkedList<SuperMenu_MenuItem*>*>();

    LinkedList<int> *layeredCurrentSelectedItem = new LinkedList<int>;
    LinkedList<int> *layeredCurrentMenuPosition = new LinkedList<int>;

    int layer1Item = 1;
    int layer = 0;

  public:
    SuperMenu_Menu(SSD1306Wire *displayer);
    void init();
    void addToMainMenu(SuperMenu_MenuItem* menuItem);
    int getCurrentSelectedItem();
    String getMenuFlow();
    
    void drawMenu();
    void drawOutline();
    
    void moveDown();
    void moveUp();
    void goNextLayer();
    void goBackLayer();

    void loop();

    int getCurrentSelectedMenu();
};

#endif
