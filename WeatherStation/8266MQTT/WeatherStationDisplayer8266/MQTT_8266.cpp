#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <OneButton.h>
#include <FS.h>
#include <LinkedList.h>
#include <ArduinoJson.h>
#include "8266WeatherDisplay.h"


#include "MQTT_8266.h"
#include "Utils_8266.h"
WiFiClient espClient;
PubSubClient client(espClient);
PubSubClient* _client = &client;
DynamicJsonDocument doc(200);
WeatherDisplay weatherDisplay = WeatherDisplay();

void displayWeather(const char *temperature, const char *humidity, const char *light, const char *pressure, const char *wind, const char* rain) {
  char t[strlen(temperature)];
  char h[strlen(humidity)];
  char l[strlen(light)];
  char p[strlen(pressure)];
  char w[strlen(wind)];
  char r[strlen(rain)];
  strcpy(t, temperature);
  strcpy(h, humidity);
  strcpy(l, light);
  strcpy(p, pressure);
  strcpy(w, wind);
  strcpy(r, rain);
  weatherDisplay.displayData(t, h, l, w, p, r);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String receivedTopic = String(topic);
  String compareTopicSensor = "homedevice/sensor/WeatherStationSensor/data";
  Serial.println("callback");
  Serial.println(receivedTopic);
  char payloadChar[length];
  for(int i = 0; i < length; i++) {
    payloadChar[i] = char(payload[i]);
  }
  String receivedPayload = String(payloadChar);
  Serial.println(receivedPayload);
  Serial.println(receivedTopic.indexOf("binary_sensor") > 0);
  if (receivedTopic.indexOf("binary_sensor") > 0) {
    Serial.println("binary_sensor");
    if (payloadChar[0] == 'O' && payloadChar[1] == 'N') {
      Utils_8266::_r = String("true");
    } else {
      Utils_8266::_r = String("false");
    }
    
  } else {
    Serial.println("sensor");
    deserializeJson(doc, receivedPayload);
    float temperature = doc["temperature"];
    int humidity = doc["humidity"];
    int pressure = doc["pressure"];
    int light = doc["light"];
    int wind_speed = doc["wind_speed"];
    Utils_8266::_t = String(temperature);
    Utils_8266::_h = String(humidity);
    Utils_8266::_l = String(light);
    Utils_8266::_p = String(pressure);
    Utils_8266::_w = String(wind_speed); 
  }
  
  Serial.println("--------sensor-------");
  Serial.println(Utils_8266::_t);
  Serial.println(Utils_8266::_h);
  Serial.println(Utils_8266::_l);
  Serial.println(Utils_8266::_p);
  Serial.println(Utils_8266::_w);
  Serial.println(Utils_8266::_r);
  Serial.println("---------------");
  displayWeather(Utils_8266::_t.c_str(), Utils_8266::_h.c_str(), Utils_8266::_l.c_str(), Utils_8266::_p.c_str(), Utils_8266::_w.c_str(), Utils_8266::_r.c_str());
}
void callbackRain(char* topic, byte* payload, unsigned int length) {
  String receivedTopic = String(topic);
  String compareTopic = "homedevice/binary_sensor/WeatherStationSensor/data";
  Serial.println("callback Rain");
  Serial.println(receivedTopic);
  
  if (!receivedTopic.compareTo(compareTopic)) {
    return;
  }
  char payloadChar[length];
  for(int i = 0; i < length; i++) {
    payloadChar[i] = char(payload[i]);
  }
  String receivedPayload = String(payloadChar);
  Serial.println(receivedPayload);
  if (payloadChar[0] == 'O' && payloadChar[1] == 'N') {
    Utils_8266::_r = String("0");
  } else {
    Utils_8266::_r = String("1");
  }
  Serial.println("--------binary_sensor-------");
  Serial.println(Utils_8266::_t);
  Serial.println(Utils_8266::_h);
  Serial.println(Utils_8266::_l);
  Serial.println(Utils_8266::_p);
  Serial.println(Utils_8266::_w);
  Serial.println(Utils_8266::_r);
  Serial.println("---------------");
  displayWeather(Utils_8266::_t.c_str(), Utils_8266::_h.c_str(), Utils_8266::_l.c_str(), Utils_8266::_p.c_str(), Utils_8266::_w.c_str(), Utils_8266::_r.c_str());
}
void MQTT_8266::init(String host, int port, String clientId, String clientPwd) {
  weatherDisplay.init();
  _MQTT_HOST = host;
  _MQTT_PORT = port;
  _MQTT_CLIENT_ID = clientId;
  _MQTT_CLIENT_PWD = clientPwd;
  _client->setServer(_MQTT_HOST.c_str(), _MQTT_PORT);
  _client->setCallback(callback);
}
PubSubClient* MQTT_8266::getClient() {
  return _client;
}
MQTT_8266::MQTT_8266(String deviceName, String type) {
  _deviceName = deviceName;
  _type = type;

  _discoverHead = Utils_8266::DISCOVER_HEAD;
  _HEADBEAT_CHANNEL = _discoverHead + "/"+ _type + "/" + _deviceName + "/state";
  _SEND_CHANNEL = "homedevice/"+ _type + "/" + _deviceName + "/data";
  _RECEIVE_CHANNEL = _SEND_CHANNEL;
  _DISCOVER_CHANNEL_BASE = _discoverHead + "/"+ _type + "/" + _deviceName;

  _isSuccessConnected = false;

}
void MQTT_8266::publishContentTemperature() {
  String publishData = "{\"device_class\": \"temperature\", \"name\": \"Temperature\", \"state_topic\": \"" + _SEND_CHANNEL + "\", \"unit_of_measurement\": \"°C\", \"value_template\": \"{{ value_json.temperature}}\" }";
  Serial.println("publish discover temperature");
  Serial.println(publishData);
  _client->publish((_DISCOVER_CHANNEL_BASE + "T" + "/config").c_str(), publishData.c_str());
}
void MQTT_8266::publishContentHumidity() {
  String publishData = "{\"device_class\": \"humidity\", \"name\": \"Humidity\", \"state_topic\": \"" + _SEND_CHANNEL + "\", \"unit_of_measurement\": \"%\", \"value_template\": \"{{ value_json.humidity}}\" }";
  Serial.println("publish discover humidity");
  Serial.println(publishData);
  _client->publish((_DISCOVER_CHANNEL_BASE + "H" + "/config").c_str(), publishData.c_str());
}
void MQTT_8266::publishContentPressure() {
  String publishData = "{\"device_class\": \"pressure\", \"name\": \"Pressure\", \"state_topic\": \"" + _SEND_CHANNEL + "\", \"unit_of_measurement\": \"hPa\", \"value_template\": \"{{ value_json.pressure}}\" }";
  Serial.println("publish discover pressure");
  Serial.println(publishData);
  _client->publish((_DISCOVER_CHANNEL_BASE + "P" + "/config").c_str(), publishData.c_str());
}
void MQTT_8266::publishContentLight() {
  String publishData = "{\"device_class\": \"illuminance\", \"name\": \"Light\", \"state_topic\": \"" + _SEND_CHANNEL + "\", \"unit_of_measurement\": \"lx\", \"value_template\": \"{{ value_json.light}}\" }";
  Serial.println("publish discover light");
  Serial.println(publishData);
  _client->publish((_DISCOVER_CHANNEL_BASE + "L" + "/config").c_str(), publishData.c_str());
}
void MQTT_8266::publishContentWindSpeed() {
  String publishData = "{\"device_class\": \"power\", \"name\": \"Wind Speed\", \"state_topic\": \"" + _SEND_CHANNEL + "\", \"unit_of_measurement\": \"mph\", \"value_template\": \"{{ value_json.wind_speed}}\" }";
  Serial.println("publish discover wind_speed");
  Serial.println(publishData);
  _client->publish((_DISCOVER_CHANNEL_BASE + "W" + "/config").c_str(), publishData.c_str());
}
void MQTT_8266::publishContentRain() {
  String publishData = "{\"device_class\": \"moisture\", \"name\": \"Rain\", \"state_topic\": \"" + _discoverHead + "/binary_sensor/" + _deviceName + "/data" + "\" }";
  Serial.println("publish discover is rain");
  Serial.println(publishData);
  _client->publish((_discoverHead + "/binary_sensor/" + _deviceName + "R/config").c_str(), publishData.c_str());
}
void MQTT_8266::processInLoop() {
  reconnect();
  _client->loop();
}
void MQTT_8266::reconnect() {
  Serial.println("reconnect MQTT Server");
  // Loop until we're reconnected
  int i = 0;
  Serial.println("a");
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
      Serial.println("Reciver Channel");
      Serial.println(_RECEIVE_CHANNEL);
      _client->subscribe(_RECEIVE_CHANNEL.c_str());
      _client->subscribe("homedevice/binary_sensor/WeatherStationSensor/data");
      
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
//  String publishStr = "{\"status\":\"";
//  publishStr.concat(client->state());
//  publishStr.concat("\", \"time\":");
//  publishStr.concat(millis());
//  publishStr.concat("}");
//  Serial.print("Publish heart beat: ");
//  Serial.print(publishStr);
//  Serial.println();
  // client->publish(HEADBEAT_CHANNEL, publishStr.c_str());
  client.publish(_HEADBEAT_CHANNEL.c_str(), "online");
  Serial.print("Publish heart beat: ");
  Serial.print(_HEADBEAT_CHANNEL);
  Serial.print(":");
  Serial.print("online");
  Serial.println();
  
}
MQTT_8266::~MQTT_8266() {
  
}
void MQTT_8266::publish_data(String s) {
  Serial.println("Publishing to channel:" + _SEND_CHANNEL);
  Serial.println("Publishing data:" + s);
  client.publish(_SEND_CHANNEL.c_str(), s.c_str());
}
void MQTT_8266::publish_binary_sensor_data(String s) {
  String sendchannel = _discoverHead + "/binary_sensor/" + _deviceName + "/data";
  Serial.println("Publishing to channel:" + sendchannel);
  Serial.println("Publishing data:" + s);
  client.publish(sendchannel.c_str(), s.c_str());
}

MQTT_Device::MQTT_Device(String deviceClass, String name, String label, String unit, String dataField, String channel) {
  _deviceName = name;
  _unit = unit;
  _label = label;
  _dataField = dataField;
  _deviceClass = deviceClass;
  _channel = channel;
}
String MQTT_Device::getConfigJSON() {
  return "{\"device_class\": \"" + _deviceClass + "\", \"name\": \"" + _deviceName + "\", \"state_topic\": \"" + _channel + "\", \"unit_of_measurement\": \"" + _unit + "\", \"value_template\": \"{{ value_json." + _dataField + "}}\" }";
}


MQTT_DeviceGroup::MQTT_DeviceGroup(String deviceName, String type) {
  _deviceName = deviceName;
  _type = type;
  
  String discoverHead = Utils_8266::DISCOVER_HEAD;
  _HEADBEAT_CHANNEL = discoverHead + "/"+ _type + "/" + _deviceName + "/state";
  _SEND_CHANNEL = discoverHead + "/"+ _type + "/" + _deviceName + "/data";
  _RECEIVE_CHANNEL = _SEND_CHANNEL;
  _DISCOVER_CHANNEL_BASE = discoverHead + "/"+ _type + "/" + _deviceName;
  _GROUPED_CONFIG_DEVICEList = LinkedList<MQTT_Device*>();
}
void MQTT_DeviceGroup::addConfig(String deviceClass, String name, String label, String unit, String dataField) {
  MQTT_Device* device = new MQTT_Device(deviceClass, name, label, unit, dataField, _SEND_CHANNEL);
  _GROUPED_CONFIG_DEVICEList.add(device);
}
void MQTT_DeviceGroup::init(String host, int port, String clientId, String clientPwd) {
  _MQTT_HOST = host;
  _MQTT_PORT = port;
  _MQTT_CLIENT_ID = clientId;
  _MQTT_CLIENT_PWD = clientPwd;
  _client->setServer(_MQTT_HOST.c_str(), _MQTT_PORT);
//  _client->setCallback(callback);
}
void MQTT_DeviceGroup::publishConfigs() {
  MQTT_Device* device;
  for(int i = 0; i < _GROUPED_CONFIG_DEVICEList.size(); i++){
    // Get device from list
    device = _GROUPED_CONFIG_DEVICEList.get(i);
    String configJSON = device->getConfigJSON();
    Serial.println("publish discover " + device->getName());
    Serial.println(configJSON.c_str());
    _client->publish((_DISCOVER_CHANNEL_BASE + "_" + device->getName() + "/config").c_str(), configJSON.c_str());
  }
}
