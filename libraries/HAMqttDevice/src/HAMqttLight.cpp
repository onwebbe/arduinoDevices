#include "HAMqttLight.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

HAMqttLight::HAMqttLight(PubSubClient* client, uint8_t pin, String deviceName, String logChannel):HAMqttDeviceBase(client, "light", deviceName, logChannel) {
  HAMqttLight(client, pin, deviceName, "", logChannel);
}

HAMqttLight::HAMqttLight(PubSubClient* client, uint8_t pin, String deviceName, String haDeviceName, String logChannel):HAMqttDeviceBase(client, "light", deviceName, haDeviceName, logChannel) {
  _pin = pin;
  
  _deviceSwitchStatusChannel_to_server = _channelHeading + "/light/" + haDeviceName + "-" + _deviceName + "/light/status";
  _deviceSwitchCommandChannel_from_server = _channelHeading + "/light/" + haDeviceName + "-" + _deviceName + "/light/set";
  _deviceBrightStatusChannel_to_server = _channelHeading + "/light/" + haDeviceName + "-" + _deviceName + "/brightness/status";
  _deviceBrightCommandChannel_from_server = _channelHeading + "/light/" + haDeviceName + "-" + _deviceName + "/brightness/set";

  // _deviceAutoDiscoverData = "{\"dev_cla\":\"light\",\"name\":\""+haDeviceName + "-" + _deviceName+"\",\"state_topic\":\""+_deviceSwitchStatusChannel_to_server+"\",\"command_topic\":\""+_deviceSwitchCommandChannel_from_server+"\",\"bri_stat_t\":\""+_deviceBrightStatusChannel_to_server+"\",\"bri_cmd_t\":\""+_deviceBrightCommandChannel_from_server+"\",\"payload_on\":\"on\",\"payload_off\":\"off\",\"optimistic\":false,\"unique_id\":\""+haDeviceName + "-" + _deviceName+"\"";
  _deviceAutoDiscoverData = "{\"dev_cla\":\"light\",\"name\":\""+_deviceName+"_"+haDeviceName+"\",\"state_topic\":\""+_deviceSwitchStatusChannel_to_server+"\",\"command_topic\":\""+_deviceSwitchCommandChannel_from_server+"\",\"bri_stat_t\":\""+_deviceBrightStatusChannel_to_server+"\",\"bri_cmd_t\":\""+_deviceBrightCommandChannel_from_server+"\",\"payload_on\":\"on\",\"payload_off\":\"off\",\"optimistic\":false,\"unique_id\":\""+haDeviceName + "-" + _deviceName+"\"";
  if (!_haDeviceName.equals("")) {
    _deviceAutoDiscoverData = _deviceAutoDiscoverData + ",\"device\":{\"identifiers\":[\""+_haDeviceName+"\"],\"name\":\"" + _haDeviceName + "\"}";
  }
  _deviceAutoDiscoverData = _deviceAutoDiscoverData + "}";

  
}

void HAMqttLight::setBrightness(int brightness) {
  _brightness = brightness;
}
void HAMqttLight::setFullPowerBeforeLightChange(int time) {
  _fullPowerBeforeLightChange = time;
}

void HAMqttLight::callback(String topicString, String payloadString) {
  log(INFO, "get call back:topic:" + topicString + ":data:" + payloadString);
  if (topicString.equals(_deviceSwitchCommandChannel_from_server)) {
    if (payloadString.equals("on")) {
      _isOn = true;
      if (_delaySwitchOff > 0)
        _delayStart = millis();
      analogWrite(_pin, _brightness);
    } else {
      _isOn = false;
      if (_delaySwitchOff > 0)
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
    if (_fullPowerBeforeLightChange > 0) {
      digitalWrite(_pin, HIGH);
      delay(_fullPowerBeforeLightChange);
    }
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
      if (command.equals("setDelaySwitchOffTime")) {
        _delaySwitchOff = commandData.toInt();
        Serial.printf("Set delay switch off time to: %d", _debugLevel);
        log(INFO, "Set delay switch off time to: " + String(_delaySwitchOff));
      } else {
        processCallbackCommandCommon(topicString, payloadString);
      }
    }
  }
}

void HAMqttLight::publishSwitchStatus() {
  _client->publish(_deviceSwitchStatusChannel_to_server.c_str(), _isOn?"on":"off");
  Serial.print("Publish status-switch:");
  Serial.println(_isOn?"on":"off");
  log(INFO, "Publish status-switch:" + _isOn?"on":"off");
}

void HAMqttLight::publishBrightStatus() {
  String pumbValueStr(_brightness);
  char pumbValueChar[pumbValueStr.length()];
  pumbValueStr.toCharArray(pumbValueChar, pumbValueStr.length() + 1);
  _client->publish(_deviceBrightStatusChannel_to_server.c_str(), pumbValueChar);
  Serial.print("Publish status-brightness:");
  Serial.println(pumbValueChar);
  log(INFO, "Publish status-brightness:" + String(_brightness));
}

void HAMqttLight::beforeReconnect() {
  log(ERROR, "[Disconnected] beforeDisconnect-Set pin to 0");
  Serial.println("afterDisconnect-Set pin to 0");
  digitalWrite(_pin, LOW);
  if (_delaySwitchOff > 0)
    _delayStart = -1;
  _isOn = false;
}

void HAMqttLight::afterRconnected() {
  Serial.print("[Disconnected] afterReconnect-Set pin to ");
  Serial.println(_brightness);
  log(ERROR, "afterReconnect-Set pin to " + _brightness);
  if (_fullPowerBeforeLightChange > 0) {
    digitalWrite(_pin, HIGH);
    delay(_fullPowerBeforeLightChange);
  }
  analogWrite(_pin, _brightness);
  if (_brightness > 0) {
    if (_delaySwitchOff > 0)
      _delayStart = millis();
    _isOn = true;
  } else {
    if (_delaySwitchOff > 0)
      _delayStart = -1;
    _isOn = false;
  }
  publishSwitchStatus();
  publishBrightStatus();
}

void HAMqttLight::loop() {
  long current = millis();
  if ((current - _recordTime) > 60000 || (current - _recordTime) < 0) {
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
void HAMqttLight::setup() {
  Serial.print("setup - Set pin to ");
  Serial.println(0);
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  
}

void HAMqttLight::doSubscribe() {
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
void HAMqttLight::setDelaySwichOff(int delayMillis) {
  _delaySwitchOff = delayMillis;
}
int HAMqttLight::getBrightness() {
  return _brightness;
}
bool HAMqttLight::isSwitchOn() {
  return _isOn;
}
