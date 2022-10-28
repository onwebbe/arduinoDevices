
#include "DataDefinitions.h"
#include <ArduinoJson.h>
#include <Arduino.h>

ControllerData::ControllerData() {
  leftController_UP_DOWN_Rate = 1;
  leftController_LEFT_RIGHT_Rate = 1;
  rightController_UP_DOWN_Rate = 1;
  rightController_LEFT_RIGHT_Rate = 1;
  switchA_Rate = 1;
  switchB_Rate = 1;
}
void ControllerData::getConfigFromJSONObj(JsonObject data) {
  leftController_UP_DOWN_Rate = data["left_UD_C"];
  leftController_LEFT_RIGHT_Rate = data["left_LR_C"];
  rightController_UP_DOWN_Rate = data["right_UD_C"];
  rightController_LEFT_RIGHT_Rate = data["right_LR_C"];
  switchA_Rate = data["A_C"];
  switchB_Rate = data["B_C"];
}
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

void ControllerData::getFromJSONObj(JsonObject json) {
  JsonObject controllerData = json["controllerData"];
  leftController_UP_DOWN = controllerData["left_UD"];
  leftController_LEFT_RIGHT = controllerData["left_LR"];
  rightController_UP_DOWN = controllerData["right_UD"];
  rightController_LEFT_RIGHT = controllerData["right_LR"];
  switchA = controllerData["A"];
  switchB = controllerData["B"];
//  for (JsonPair kv : controllerData) {
//    Serial.println(kv.key().c_str());
//    Serial.println(kv.value().as<double>());
//  }
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
