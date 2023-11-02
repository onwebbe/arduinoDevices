#include "MenuItem.h"
#include <LinkedList.h>

SuperMenu_MenuItem::SuperMenu_MenuItem(String text, int layer, SuperMenu_MenuItem* par) {
  this->menuText = text;
  this->layer = layer;
  this->parent = par;
}
SuperMenu_MenuItem::SuperMenu_MenuItem(String text, int layer) {
  this->menuText = text;
  this->layer = layer;
}


void SuperMenu_MenuItem::setIndex(int idx) {
  this->index = idx;
}
void SuperMenu_MenuItem::setMenuText(String text) {
  this->menuText = text;
}
void SuperMenu_MenuItem::setLayer(int layer) {
  this->layer = layer;
}
void SuperMenu_MenuItem::setParent(SuperMenu_MenuItem* par) {
  this->parent = par;
}
int SuperMenu_MenuItem::getIndex() {
  return this->index;
}
String SuperMenu_MenuItem::getMenuText() {
  return this->menuText;
}
int SuperMenu_MenuItem::getLayer() {
  return this->layer;
}
SuperMenu_MenuItem* SuperMenu_MenuItem::getParent() {
  return this->parent;
}

void SuperMenu_MenuItem::addSubMenu(SuperMenu_MenuItem* menu) {
  this->subMenuList->add(menu);
}
LinkedList<SuperMenu_MenuItem*>* SuperMenu_MenuItem::getSubMenuList() {
  return this->subMenuList;
}
SuperMenu_MenuItem* SuperMenu_MenuItem::getSubMenu(int idx) {
  if (idx < this->subMenuList->size()) {
    return this->subMenuList->get(idx);
  } else {
    return NULL;
  }
}
