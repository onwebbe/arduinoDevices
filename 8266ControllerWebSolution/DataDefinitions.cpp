
#include "DataDefinitions.h"
#include <ArduinoJson.h>
#include <Arduino.h>
String ControllerData::convertToJSON() {
  StaticJsonDocument<200> doc;
  JsonObject data = doc.createNestedObject("controllerData");
  data["left_UD"] = leftController_UP_DOWN;
  data["left_LR"] = leftController_LEFT_RIGHT;
  data["right_UD"] = rightController_UP_DOWN;
  data["right_LR"] = rightController_LEFT_RIGHT;
  data["A"] = switchA;
  data["B"] = switchB;
  serializeJson(doc, Serial);
  String resultString = "";
  serializeJsonPretty(doc, resultString);
  return resultString;
}

void ControllerData::getFromJSON(String json) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, json);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  const JsonObject controllerData = doc["controllerData"];
  leftController_UP_DOWN = controllerData["left_UD"];
  leftController_LEFT_RIGHT = controllerData["left_LR"];
  rightController_UP_DOWN = controllerData["right_UD"];
  rightController_LEFT_RIGHT = controllerData["right_LR"];
  switchA = controllerData["A"];
  switchB = controllerData["B"];
}


String SensorData::convertToJSON() {
  StaticJsonDocument<200> doc;
  JsonObject data = doc.createNestedObject("sensorData");
  data["X"] = rotateX;
  data["Y"] = rotateY;
  data["Z"] = rotateZ;
  data["distance"] = distance;
  serializeJson(doc, Serial);
  String resultString = "";
  serializeJsonPretty(doc, resultString);
  return resultString;
}
void SensorData::getFromJSON(String json) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, json);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  const JsonObject sensorData = doc["sensorData"];
  rotateX = sensorData["X"];
  rotateY = sensorData["Y"];
  rotateZ = sensorData["Z"];
  distance = sensorData["distance"];
}
