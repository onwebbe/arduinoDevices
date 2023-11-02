#ifndef __SUPERMENU_LAYEREDMENU_H
#define __SUPERMENU_LAYEREDMENU_H
#include "MenuItem.h"
#include <LinkedList.h>
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"

class SuperLayeredMenu{
  private:
    SSD1306Wire *display;
    LinkedList<SuperMenu_Menu*> layeredMenu = LinkedList<SuperMenu_Menu*>();
    SuperMenu_Menu *currentMenu;
  public:
    SuperLayeredMenu(SSD1306Wire *displayer);
    void init();
    void goBackLayer();

    void loop();
};

#endif
