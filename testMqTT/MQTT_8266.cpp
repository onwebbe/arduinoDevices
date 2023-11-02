#include "MQTT_8266.h"
#include "Utils_8266.h"
#include "MQTT_DeviceGroup.h"
#include "MQTT_Device.h"

#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <OneButton.h>
#include <FS.h>
#include <LinkedList.h>
#include <ArduinoJson.h>


DynamicJsonDocument doc(200);

void MQTT_8266::init(String host, int port, String clientId, String clientPwd) {
  _MQTT_HOST = host;
  _MQTT_PORT = port;
  _MQTT_CLIENT_ID = clientId;
  _MQTT_CLIENT_PWD = clientPwd;
  WiFiClient espClient;
  _client = new PubSubClient(espClient);
  _client->setServer(_MQTT_HOST.c_str(), _MQTT_PORT);
//  _client->setCallback(callback);
}
PubSubClient* MQTT_8266::getClient() {
  return _client;
}
MQTT_8266::MQTT_8266(String deviceName, String type) {
  _isSuccessConnected = false;
  _mqtt_DeviceGroup = LinkedList<MQTT_DeviceGroup*>();
}
void MQTT_8266::addDeviceGroup(MQTT_DeviceGroup* group) {
  _mqtt_DeviceGroup.add(group);
}
void MQTT_8266::processInLoop() {
  reconnect();
  _client->loop();
}
void MQTT_8266::reconnect() {
  Serial.println("reconnect MQTT Server");
  // Loop until we're reconnected
  int i = 0;
  Serial.println(_client->connected());
  while (!_client->connected()) {
    Serial.println("b");
    Serial.println(_client->connected());
    if (i >= 5) {
      if (_isSuccessConnected) {
        Utils_8266::restartOnly();
      } else {
        Utils_8266::resetNetworkSettingsWhenFailed();
      }
    }
    Serial.print("Attempting MQTT connection...");
    Serial.print(i);
    // Attempt to connect
    Serial.println(_MQTT_CLIENT_ID);
    Serial.println(_MQTT_CLIENT_PWD);
    if (_client->connect(_MQTT_CLIENT_ID.c_str(), "", _MQTT_CLIENT_PWD.c_str())) {
      Serial.println("c");
      Serial.println(_client->connected());
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
//      Serial.println("Reciver Channel");
//      Serial.println(_RECEIVE_CHANNEL);
//      _client->subscribe(_RECEIVE_CHANNEL.c_str());
//      _client->subscribe("homedevice/binary_sensor/WeatherStationSensor/data");
      
      if (!SPIFFS.begin()) {
        Serial.println("LittleFS mount failed");
        return;
      }
      SPIFFS.remove("/network.txt");
      Serial.print("Write to file");
      Serial.print("/network.txt with content:");
      Serial.print(_MQTT_CLIENT_ID + "," + _MQTT_CLIENT_PWD);
      Utils_8266::writeFile("/network.txt", (_MQTT_CLIENT_ID + "," + _MQTT_CLIENT_PWD).c_str());
      SPIFFS.end();
      _isSuccessConnected = true;
    } else {
      Serial.print("failed, rc=");
      Serial.print(_client->state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    i = i + 1;
  }
}

void MQTT_8266::heartBeat() {
  
}
MQTT_8266::~MQTT_8266() {
  delete(&_mqtt_DeviceGroup);
}
void MQTT_8266::publish_data(String s) {
  Serial.println("Publishing");

}
void MQTT_8266::publish_config() {
  Serial.println("Publishing Config");
  Serial.println("Publishing data:");
  for(int i = 0; i < _mqtt_DeviceGroup.size(); i++) {
    MQTT_DeviceGroup* deviceGroup = _mqtt_DeviceGroup.get(i);
    deviceGroup->publishConfigs(_client);
  }
}
