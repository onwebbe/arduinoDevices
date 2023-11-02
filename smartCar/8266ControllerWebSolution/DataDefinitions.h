#ifndef _CONTROLLER_DATA_DEFINITIONS_H__
#define _CONTROLLER_DATA_DEFINITIONS_H__
#include <Arduino.h>
#include <ArduinoJson.h>
class ControllerData {
  public:
    byte leftController_UP_DOWN;
    byte leftController_LEFT_RIGHT;
    byte rightController_UP_DOWN;
    byte rightController_LEFT_RIGHT;
    byte switchA;
    byte switchB;
    double leftController_UP_DOWN_Rate;
    double leftController_LEFT_RIGHT_Rate;
    double rightController_UP_DOWN_Rate;
    double rightController_LEFT_RIGHT_Rate;
    double switchA_Rate;
    double switchB_Rate;
  public:
    ControllerData();
    void getFromJSONObj(JsonObject json);
    void getConfigFromJSONObj(JsonObject json);
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
