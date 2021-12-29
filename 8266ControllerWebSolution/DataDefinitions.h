#ifndef _CONTROLLER_DATA_DEFINITIONS_H__
#define _CONTROLLER_DATA_DEFINITIONS_H__
#include <Arduino.h>
class ControllerData {
  public:
    byte leftController_UP_DOWN;
    byte leftController_LEFT_RIGHT;
    byte rightController_UP_DOWN;
    byte rightController_LEFT_RIGHT;
    byte switchA;
    byte switchB;
  public:
    void getFromJSON(String json);
    String convertToJSON();
};


class SensorData {
  public:
    byte rotateX;
    byte rotateY;
    byte rotateZ;
    byte distance;
   public:
    void getFromJSON(String json);
    String convertToJSON();
  
};
#endif
