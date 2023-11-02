#include "MyMqttLight.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

MqttLight::MqttLight(PubSubClient* client, uint8_t pin, String deviceName, String logChannel) {
  MqttLight(client, pin, deviceName, "", logChannel);
}

MqttLight::MqttLight(PubSubClient* client, uint8_t pin, String deviceName, String haDeviceName, String logChannel) {
  _deviceName = deviceName;
  _pin = pin;
  _client = client;
  _haDeviceName = deviceName;
  
  _deviceSwitchStatusChannel_to_server = _channelHeading + "/light/" + _deviceName + "/light/status";
  _deviceSwitchCommandChannel_from_server = _channelHeading + "/light/" + _deviceName + "/light/set";
  _deviceBrightStatusChannel_to_server = _channelHeading + "/light/" + _deviceName + "/brightness/status";
  _deviceBrightCommandChannel_from_server = _channelHeading + "/light/" + _deviceName + "/brightness/set";
  _deviceDataChannel_to_server =  _channelHeading + "/light/" + _deviceName + "/command/data";
  _deviceCommandChannel_from_server = _channelHeading + "/light/" + _deviceName + "/command/set";
  _deviceAutoDiscoverChannel_to_server = _channelHeading + "/light/" + _deviceName + "/config";
  _deviceAutoDiscoverData = "{\"dev_cla\":\"light\",\"name\":\""+_deviceName+"\",\"state_topic\":\""+_deviceSwitchStatusChannel_to_server+"\",\"command_topic\":\""+_deviceSwitchCommandChannel_from_server+"\",\"bri_stat_t\":\""+_deviceBrightStatusChannel_to_server+"\",\"bri_cmd_t\":\""+_deviceBrightCommandChannel_from_server+"\",\"payload_on\":\"on\",\"payload_off\":\"off\",\"optimistic\":false,\"unique_id\":\""+_deviceName+"\"";
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
void MqttLight::setHeading(String heading) {
  _channelHeading = heading;
}

void MqttLight::setBrightness(int brightness) {
  _brightness = brightness;
}

void MqttLight::callback(String topicString, String payloadString) {
  log(INFO, "get call back:topic:" + topicString + ":data:" + payloadString);
  if (topicString.equals(_deviceSwitchCommandChannel_from_server)) {
    if (payloadString.equals("on")) {
      _isOn = true;
      _delayStart = millis();
      analogWrite(_pin, _brightness);
    } else {
      _isOn = false;
      _delayStart = -1;
      digitalWrite(_pin, LOW);
    }
    Serial.print("callback-switch-Set pin to ");
    Serial.println(_brightness);
    log(INFO, "callback-switch-Set pin to " + _brightness);
    publishSwitchStatus();
    publishBrightStatus();
  } else if (topicString.equals(_deviceBrightCommandChannel_from_server)) {
    _brightness = payloadString.toInt();
    if (_brightness == 0) {
      _isOn = false;
      _delayStart = -1;
    } else {
      _isOn = true;
      _delayStart = millis();
    }
    Serial.print("callback-brightness-Set pin to ");
    Serial.println(_brightness);
    analogWrite(_pin, _brightness);
    log(INFO, "callback-brightness-Set pin to " + _brightness);
    publishBrightStatus();
  } else if (topicString.equals(_deviceCommandChannel_from_server)) {
    if (payloadString.equals("getStatus")) {
      String isOn = _isOn?"on":"off";
      String data = "{\"lightSwitch\":\"" + isOn + "\",\"lightBrightness\": " + _brightness + "}";
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
        Serial.printf("Set debug level to: %d", _debugLevel);
        log(INFO, "Set debug level to: " + String(_debugLevel));
      } else if (command.equals("setDelaySwitchOffTime")) {
        _delaySwitchOff = commandData.toInt();
        Serial.printf("Set delay switch off time to: %d", _debugLevel);
        log(INFO, "Set delay switch off time to: " + String(_delaySwitchOff));
      }
    }
  }
}

void MqttLight::publishSwitchStatus() {
  _client->publish(_deviceSwitchStatusChannel_to_server.c_str(), _isOn?"on":"off");
  Serial.print("Publish status-switch:");
  Serial.println(_isOn?"on":"off");
  log(INFO, "Publish status-switch:" + _isOn?"on":"off");
}

void MqttLight::publishBrightStatus() {
  String pumbValueStr(_brightness);
  char pumbValueChar[pumbValueStr.length()];
  pumbValueStr.toCharArray(pumbValueChar, pumbValueStr.length() + 1);
  _client->publish(_deviceBrightStatusChannel_to_server.c_str(), pumbValueChar);
  Serial.print("Publish status-brightness:");
  Serial.println(pumbValueChar);
  log(INFO, "Publish status-brightness:" + _brightness);
}

void MqttLight::beforeReconnect() {
  log(ERROR, "[Disconnected] beforeDisconnect-Set pin to 0");
  Serial.println("afterDisconnect-Set pin to 0");
  digitalWrite(_pin, LOW);
  _delayStart = -1;
  _isOn = false;
}

void MqttLight::afterRconnected() {
  Serial.print("[Disconnected] afterReconnect-Set pin to ");
  Serial.println(_brightness);
  log(ERROR, "afterReconnect-Set pin to " + _brightness);
  analogWrite(_pin, _brightness);
  if (_brightness > 0) {
    _delayStart = millis();
    _isOn = true;
  } else {
    _delayStart = -1;
    _isOn = false;
  }
  publishSwitchStatus();
  publishBrightStatus();
}

void MqttLight::log(int level, String s) {
  String levelString = "ERROR";
  switch(level) {
    case 1: levelString = "ERROR";break;
    case 2: levelString = "INFO";break;
    case 3: levelString = "DEBUG";break;
  }
  if (level > 0 && level <= _debugLevel) {
    String logString = "LIGHT:" + _deviceName + ":" + levelString + ":" + s;
    Serial.print("Sending log:");
    Serial.print(_deviceLogChannel_to_server);
    Serial.print(":");
    Serial.println(logString);
    _client->publish(_deviceLogChannel_to_server.c_str(), logString.c_str());
  }
}

void MqttLight::publishDiscover() {
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

void MqttLight::loop() {
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
void MqttLight::setup() {
  Serial.print("setup - Set pin to ");
  Serial.println(0);
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
}

void MqttLight::doSubscribe() {
  Serial.print("Subscribing:");
  Serial.println(_deviceSwitchCommandChannel_from_server);
  Serial.print("Subscribing:");
  Serial.println(_deviceBrightCommandChannel_from_server);
  Serial.print("Subscribing:");
  Serial.println(_deviceCommandChannel_from_server);
  _client->subscribe(_deviceSwitchCommandChannel_from_server.c_str());
  _client->subscribe(_deviceBrightCommandChannel_from_server.c_str());
  _client->subscribe(_deviceCommandChannel_from_server.c_str());
  log(INFO, "Subscribe:" + _deviceSwitchCommandChannel_from_server + ":" + _deviceBrightCommandChannel_from_server + ":" + _deviceCommandChannel_from_server);
}
void MqttLight::setDebugLevel(int debugLevel) {
  _debugLevel = debugLevel;
}
void MqttLight::setDelaySwichOff(int delayMillis) {
  _delaySwitchOff = delayMillis;
}
