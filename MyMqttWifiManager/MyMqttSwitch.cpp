#include "MyMqttSwitch.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
MqttSwitch::MqttSwitch(PubSubClient* client, uint8_t pin, String deviceName, String logChannel) {
  MqttSwitch(client, pin, deviceName, "", logChannel);
}

MqttSwitch::MqttSwitch(PubSubClient* client, uint8_t pin, String deviceName, String haDeviceName, String logChannel) {
  _deviceName = deviceName;
  _pin = pin;
  _client = client;
  _haDeviceName = deviceName;
  
  _deviceSwitchStatusChannel_to_server = _channelHeading + "/light/" + _deviceName + "/light/status";
  _deviceSwitchCommandChannel_from_server = _channelHeading + "/light/" + _deviceName + "/light/set";
  _deviceDataChannel_to_server =  _channelHeading + "/light/" + _deviceName + "/command/data";
  _deviceCommandChannel_from_server = _channelHeading + "/light/" + _deviceName + "/command/set";
  _deviceAutoDiscoverChannel_to_server = _channelHeading + "/light/" + _deviceName + "/config";
  _deviceAutoDiscoverData = "{\"dev_cla\":\"light\",\"name\":\""+_deviceName+"\",\"state_topic\":\""+_deviceSwitchStatusChannel_to_server+"\",\"command_topic\":\""+_deviceSwitchCommandChannel_from_server+"\",\"payload_on\":\"on\",\"payload_off\":\"off\",\"optimistic\":false,\"unique_id\":\""+_deviceName+"\"";
  if (_haDeviceName.equals("")) {
    _deviceAutoDiscoverData = _deviceAutoDiscoverData + ",\"device\":{\"identifiers\":[\""+_deviceName+"\"],\"name\":\"" + _haDeviceName + "\"}";
  }
  _deviceAutoDiscoverData = _deviceAutoDiscoverData + "}";
  _deviceLogChannel_to_server = logChannel;
  _recordTime = millis();
  log(INFO, "Mqtt Light instance created");
  log(ERROR, "Publish command to " + _deviceCommandChannel_from_server + " for action");
  Serial.print("Publish command to ");
  Serial.print(_deviceCommandChannel_from_server);
  Serial.println(" for action");
  digitalWrite(_pin, LOW);
}

void MqttSwitch::callback(String topicString, String payloadString) {
  log(INFO, "get call back:topic:" + topicString + ":data:" + payloadString);
  if (topicString.equals(_deviceSwitchCommandChannel_from_server)) {
    if (payloadString.equals("on")) {
      _isOn = true;
      _delayStart = millis();
      digitalWrite(_pin, HIGH);
    } else {
      _isOn = false;
      _delayStart = -1;
      digitalWrite(_pin, LOW);
    }
    Serial.print("callback-switch-Set pin");
    Serial.println(_isOn?"ON":"OFF");
    publishSwitchStatus();
  } else if (topicString.equals(_deviceCommandChannel_from_server)) {
    if (payloadString.equals("getStatus")) {
      String isOn = _isOn?"on":"off";
      String data = "{\"switch\":\"" + isOn + "\"}";
      _client->publish(_deviceDataChannel_to_server.c_str(), data.c_str());
    } else {
      StaticJsonDocument<768> doc;
      DeserializationError error = deserializeJson(doc, payloadString);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      String command = String(doc["cmd"]);
      String commandData = String(doc["data"]);
      if (command.equals("setDebugLevel")) {
        _debugLevel = commandData.toInt();
      } else if (command.equals("setDelaySwitchOffTime")) {
        _delaySwitchOff = commandData.toInt();
      }
    }
  }
}

void MqttSwitch::publishSwitchStatus() {
  _client->publish(_deviceSwitchStatusChannel_to_server.c_str(), _isOn?"on":"off");
  Serial.print("Publish status-switch:");
  Serial.println(_isOn?"on":"off");
  log(INFO, "Publish status-switch:" + _isOn?"on":"off");
}

void MqttSwitch::beforeReconnect() {
  log(ERROR, "[Disconnected] beforeDisconnect-Set pin to 0");
  Serial.println("afterDisconnect-Set pin to 0");
  digitalWrite(_pin, LOW);
  _delayStart = -1;
}

void MqttSwitch::afterRconnected() {
  Serial.print("[Disconnected] afterReconnect-Set pin to ");
  Serial.println(_isOn?"ON":"OFF");
  log(ERROR, "afterReconnect-Set pin to " + _isOn?"ON":"OFF");
  analogWrite(_pin, _isOn?HIGH:LOW);
  if (_isOn) {
    _delayStart = millis();
  } else {
    _delayStart = -1;
  }
  publishSwitchStatus();
}

void MqttSwitch::log(int level, String s) {
  if (level > 0 && level <= _debugLevel) {
    String logString = "SWITCH:" + _deviceName + ":" + s;
    Serial.print("Sending log:");
    Serial.print(_deviceLogChannel_to_server);
    Serial.print(":");
    Serial.println(logString);
    _client->publish(_deviceLogChannel_to_server.c_str(), logString.c_str());
  }
}

void MqttSwitch::publishDiscover() {
  Serial.print("Sending discover string:");
  Serial.print(_deviceAutoDiscoverChannel_to_server);
  Serial.print(":");
  Serial.println(_deviceAutoDiscoverData);
//  StaticJsonDocument<512> doc;
//  doc["device_class"] = "light";
//  doc["name"] = _deviceName;
//  doc["state_topic"] = _deviceSwitchStatusChannel_to_server;
//  doc["command_topic"] = _deviceSwitchCommandChannel_from_server;
//  doc["brightness_state_topic"] = _deviceBrightStatusChannel_to_server;
//  doc["brightness_command_topic"] = _deviceBrightCommandChannel_from_server;
//  doc["payload_on"] = "on";
//  doc["payload_off"] = "off";
//  doc["optimistic"] = false;
//  doc["unique_id"] = _deviceName;
// 
//  String data;
//  serializeJson(doc, data);
  log(INFO, "Sending discover channel:" + _deviceAutoDiscoverChannel_to_server + ":data:" + _deviceAutoDiscoverData);
  bool isSuccess = _client->publish(_deviceAutoDiscoverChannel_to_server.c_str(), _deviceAutoDiscoverData.c_str());
  log(INFO, "Sending discover:" + isSuccess?"success":"failed");
}

void MqttSwitch::loop() {
  long current = millis();
  if ((current - _recordTime) > 60000 || (current - _recordTime) < 0) {
    publishDiscover();
    _recordTime = millis();
  }
  if (_delayStart > 0 && (current - _delayStart) >= _delaySwitchOff || (current - _delayStart) < 0) {
    Serial.println("Delay switch off done");
    _delayStart = -1;
    digitalWrite(_pin, LOW);
    _isOn = false;
    publishSwitchStatus();
  }
}
void MqttSwitch::setup() {
  Serial.print("setup - Set pin to ");
  Serial.println(_isOn?"ON":"OFF");
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
}

void MqttSwitch::doSubscribe() {
  Serial.print("Subscribing:");
  Serial.println(_deviceSwitchCommandChannel_from_server);
  Serial.print("Subscribing:");
  Serial.println(_deviceCommandChannel_from_server);
  _client->subscribe(_deviceSwitchCommandChannel_from_server.c_str());
  _client->subscribe(_deviceCommandChannel_from_server.c_str());
  log(INFO, "Subscribe:" + _deviceSwitchCommandChannel_from_server + ":" + _deviceCommandChannel_from_server);
}
void MqttSwitch::setDebugLevel(int debugLevel) {
  _debugLevel = debugLevel;
}
void MqttSwitch::setDelaySwichOff(int delayMillis) {
  _delaySwitchOff = delayMillis;
}
