#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <FS.h>

#include "WIFI_NETWORK_8266.h"
#include "Utils_8266.h"

void WIFI_NETWORK_8266::saveConfigCallbackWifiManager() {
   
}
WIFI_NETWORK_8266::WIFI_NETWORK_8266(String wifiName, String defaultClientId) {
  _wifiName = wifiName;
  Serial.println("begin start littleFS");
  if (!SPIFFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }

  if (SPIFFS.exists("/network.txt")) {
    Serial.println("network setting file exists");
    String strRaw = Utils_8266::readFile("/network.txt");
    String splited[2];
    Utils_8266::Split(strRaw, ",", splited);
    _client_id = splited[0];
    _client_pwd = splited[1];
    Serial.print("get setting file");
    Serial.print(_client_id);
    Serial.println();
    Serial.print(_client_pwd);
    Serial.println();
  }
  SPIFFS.end();
  Serial.println("end start littleFS");
  
  WiFiManager wm;
  _wifiManager = &wm;
//  _wifiManager->resetSettings();
//  _wifiManager->setSaveConfigCallback(saveConfigCallbackWifiManager);
  _client_id = defaultClientId;
}
void WIFI_NETWORK_8266::resetNetwork() {
  Utils_8266::resetNetworkSettingsWhenFailed();
}
WIFI_NETWORK_8266::~WIFI_NETWORK_8266() {
  
}
bool WIFI_NETWORK_8266::init() {
  WiFiManagerParameter custom_client_id("clientid", "Client Id", _client_id.c_str(), 50);
  WiFiManagerParameter custom_client_pwd("clientpwd", "Client Pass", _client_pwd.c_str(), 100);
  _wifiManager->addParameter(&custom_client_id);
  _wifiManager->addParameter(&custom_client_pwd);
  bool res;
  Serial.print("Creating wifi:");
  Serial.print(_wifiName);
  Serial.println();
  res = _wifiManager->autoConnect(_wifiName.c_str(), ""); // password protected ap
  Serial.println("Exit wifimanager autoconnect");
  if(!res) {
      Serial.println("Failed to connect");
//      Utils_8266::resetNetworkSettingsWhenFailed();
  }
  Serial.println("connected...yeey :)");
  Serial.println(custom_client_id.getValue());
  Serial.println(custom_client_pwd.getValue());
  Serial.println("--------get wifi ssid--------");
  Serial.println(_wifiManager->getWiFiSSID());
  _client_id = custom_client_id.getValue();
  _client_pwd = custom_client_pwd.getValue();
  Serial.print("CLIENT_ID: ");
  Serial.print(_client_id);
  Serial.print(":::::");
  Serial.println();
  Serial.print("CLIENT_PASSWORD: ");
  Serial.print(_client_pwd);
  Serial.print(":::::");
  Serial.println();
}
String WIFI_NETWORK_8266::getClientId() {
  return _client_id;
}
String WIFI_NETWORK_8266::getClientPassword() {
  return _client_pwd;
}
WiFiManager* WIFI_NETWORK_8266::getWiFiManager() {
  return _wifiManager;
}
