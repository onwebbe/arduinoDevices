#include <Arduino.h>
#include <LinkedList.h>

#ifndef __SUPERMENU_MENUITEM_H
#define __SUPERMENU_MENUITEM_H
class SuperMenu_MenuItem {
  private:
    int index;
    String menuText;
    int layer;
    SuperMenu_MenuItem* parent;
    LinkedList<SuperMenu_MenuItem*>* subMenuList = new LinkedList<SuperMenu_MenuItem*>();

  public:
    SuperMenu_MenuItem(String text, int layer, SuperMenu_MenuItem* par);
    SuperMenu_MenuItem(String text, int layer);
    void setIndex(int idx);
    void setMenuText(String text);
    void setLayer(int layer);
    void setParent(SuperMenu_MenuItem* par);
    void addSubMenu(SuperMenu_MenuItem* menu);

    int getIndex();
    String getMenuText();
    int getLayer();
    SuperMenu_MenuItem* getParent();
    LinkedList<SuperMenu_MenuItem*>* getSubMenuList();
    SuperMenu_MenuItem* getSubMenu(int idx);
};

#endif
