#include "HAMqttSwitch.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

HAMqttSwitch::HAMqttSwitch(PubSubClient* client, uint8_t pin, String deviceName, String logChannel):HAMqttDeviceBase(client, "switch", deviceName, logChannel) {
  init(client, pin, deviceName, "", logChannel, false);
}

HAMqttSwitch::HAMqttSwitch(PubSubClient* client, uint8_t pin, String deviceName, String haDeviceName, String logChannel):HAMqttDeviceBase(client, "switch", deviceName, haDeviceName, logChannel) {
  init(client, pin, deviceName, haDeviceName, logChannel, false);
}

void HAMqttSwitch::init(PubSubClient* client, uint8_t pin, String deviceName, String haDeviceName, String logChannel, bool defaultValue) {
  _isOn = defaultValue;
  _pin = pin;
  
  _deviceSwitchStatusChannel_to_server = _channelHeading + "/switch/" + _haDeviceName + "-" + _deviceName + "/switch/status";
  _deviceSwitchCommandChannel_from_server = _channelHeading + "/switch/" + _haDeviceName + "-" + _deviceName + "/switch/set";
  
  // _deviceAutoDiscoverData = "{\"dev_cla\":\"switch\",\"name\":\""+_haDeviceName + "-" + _deviceName+"\",\"state_topic\":\""+_deviceSwitchStatusChannel_to_server+"\",\"command_topic\":\""+_deviceSwitchCommandChannel_from_server+"\",\"payload_on\":\"on\",\"payload_off\":\"off\",\"optimistic\":false,\"unique_id\":\""+haDeviceName + "-" + _deviceName+"\"";
_deviceAutoDiscoverData = "{\"dev_cla\":\"switch\",\"name\":\"" + _deviceName+"_"+_haDeviceName+"\",\"state_topic\":\""+_deviceSwitchStatusChannel_to_server+"\",\"command_topic\":\""+_deviceSwitchCommandChannel_from_server+"\",\"payload_on\":\"on\",\"payload_off\":\"off\",\"optimistic\":false,\"unique_id\":\""+_haDeviceName + "-" + _deviceName+"\"";
 if (!_haDeviceName.equals("")) {
    _deviceAutoDiscoverData = _deviceAutoDiscoverData + ",\"device\":{\"identifiers\":[\""+_haDeviceName+"\"],\"name\":\"" + _haDeviceName + "\"}";
  }
  _deviceAutoDiscoverData = _deviceAutoDiscoverData + "}";
}
void HAMqttSwitch::callback(String topicString, String payloadString) {
  log(INFO, "get call back:topic:" + topicString + ":data:" + payloadString);
  if (topicString.equals(_deviceSwitchCommandChannel_from_server)) {
    if (payloadString.equals("on")) {
      _isOn = true;
      if (_delaySwitchOff > 0)
        _delayStart = millis();
      digitalWrite(_pin, HIGH);
    } else {
      _isOn = false;
      if (_delaySwitchOff > 0)
        _delayStart = -1;
      digitalWrite(_pin, LOW);
    }
    Serial.print("callback-switch-Set pin");
    Serial.println(_isOn?"ON":"OFF");
    saveStatus();
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
      if (command.equals("setDelaySwitchOffTime")) {
        _delaySwitchOff = commandData.toInt();
      } else {
        processCallbackCommandCommon(topicString, payloadString);
      }
    }
  }
}

void HAMqttSwitch::publishSwitchStatus() {
  _client->publish(_deviceSwitchStatusChannel_to_server.c_str(), _isOn?"on":"off");
  Serial.print("Publish status-switch:");
  Serial.println(_isOn?"on":"off");
  log(INFO, "Publish status-switch:" + _isOn?"on":"off");
}

void HAMqttSwitch::beforeReconnect() {
  log(ERROR, "[Disconnected] beforeDisconnect-Set pin to 0");
  Serial.println("afterDisconnect-Set pin to 0");
  digitalWrite(_pin, LOW);
  if (_delaySwitchOff > 0)
    _delayStart = -1;
}

void HAMqttSwitch::afterRconnected() {
  Serial.print("[Disconnected] afterReconnect-Set pin to ");
  Serial.println(_isOn?"ON":"OFF");
  log(ERROR, "afterReconnect-Set pin to " + _isOn?"ON":"OFF");
  analogWrite(_pin, _isOn?HIGH:LOW);
  if (_isOn) {
    if (_delaySwitchOff > 0)
        _delayStart = millis();
  } else {
    if (_delaySwitchOff > 0)
        _delayStart = -1;
  }
  publishSwitchStatus();
}


void HAMqttSwitch::loop() {
  long current = millis();
  if ((current - _recordTime) > _discoverTimeGap || (current - _recordTime) < 0) {
    publishDiscover();
    _recordTime = millis();
  }
  if (_delayStart > 0 && ((current - _delayStart) >= _delaySwitchOff || (current - _delayStart) < 0)) {
    Serial.println("Delay switch off done");
    _delayStart = -1;
    digitalWrite(_pin, LOW);
    _isOn = false;
    publishSwitchStatus();
  }
}
void HAMqttSwitch::setup() {
  Serial.print("setup - Set pin to ");
  Serial.println(_isOn?"ON":"OFF");
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, _isOn);

  Serial.println("Initialing:");
  Serial.print("default value:");
  Serial.println(_isOn);
  Serial.print("_deviceSwitchStatusChannel_to_server:");
  Serial.println(_deviceSwitchStatusChannel_to_server);
  Serial.print("_deviceSwitchCommandChannel_from_server:");
  Serial.println(_deviceSwitchCommandChannel_from_server);
  Serial.print("_deviceAutoDiscoverData:");
  Serial.println(_deviceAutoDiscoverData);
}

void HAMqttSwitch::doSubscribe() {
  Serial.print("Subscribing:");
  Serial.println(_deviceSwitchCommandChannel_from_server);
  Serial.print("Subscribing:");
  Serial.println(_deviceCommandChannel_from_server);
  _client->subscribe(_deviceSwitchCommandChannel_from_server.c_str());
  _client->subscribe(_deviceCommandChannel_from_server.c_str());
  log(INFO, "Subscribe:" + _deviceSwitchCommandChannel_from_server + ":" + _deviceCommandChannel_from_server);
}

void HAMqttSwitch::setDelaySwichOff(int delayMillis) {
  _delaySwitchOff = delayMillis;
}

bool HAMqttSwitch::isSwitchOn() {
  return _isOn;
}

String HAMqttSwitch::getStatusString() {
  return String(_isOn);
}

void HAMqttSwitch::restoreStatus() {
  String statusString = getSavedStatus();
  if (statusString.length() > 0) {
    String isOnString = statusString;
    boolean isOn = (boolean)isOnString.toInt();
    _isOn = isOn;
    if (isOn) {
      if (_delaySwitchOff > 0)
        _delayStart = millis();
      digitalWrite(_pin, HIGH);
    } else {
      if (_delaySwitchOff > 0)
        _delayStart = -1;
      digitalWrite(_pin, LOW);
    }
    publishSwitchStatus();
  }
}

void HAMqttSwitch::publishStatus() {
  publishSwitchStatus();
}
