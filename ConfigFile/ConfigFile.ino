// Example: storing JSON configuration file in flash file system
//
// Uses ArduinoJson library by Benoit Blanchon.
// https://github.com/bblanchon/ArduinoJson
//
// Created Aug 10, 2015 by Ivan Grokhotkov.
//
// This example code is in the public domain.

#include <ArduinoJson.h>
#include "FS.h"

bool loadConfig() {
  
  if (SPIFFS.exists("/index.html")){
    Serial.print("/index.html");
    Serial.println(" FOUND.");
  } else {
    Serial.print("/index.html");
    Serial.print(" NOT FOUND.");
  }
  File dataFile = SPIFFS.open("/index.html", "r");
  return true;
}
//
//bool saveConfig() {
//  StaticJsonDocument<200> doc;
//  doc["serverName"] = "api.example.com";
//  doc["accessToken"] = "128du9as8du12eoue8da98h123ueh9h98";
//
//  File configFile = LittleFS.open("/config.json", "w");
//  if (!configFile) {
//    Serial.println("Failed to open config file for writing");
//    return false;
//  }
//
//  serializeJson(doc, configFile);
//  return true;
//}

void setup() {
  Serial.begin(115200);
  Serial.println("");
  delay(1000);
  Serial.println("Mounting FS...");

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }


//  if (!saveConfig()) {
//    Serial.println("Failed to save config");
//  } else {
//    Serial.println("Config saved");
//  }

  if (!loadConfig()) {
    Serial.println("Failed to load config");
  } else {
    Serial.println("Config loaded");
  }
}

void loop() {
}
