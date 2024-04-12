#include "HAMqttServoGoAndBack.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Servo.h>

HAMqttServoGoAndBack::HAMqttServoGoAndBack(PubSubClient* client, uint8_t pin, String deviceName, String logChannel):HAMqttDeviceBase(client, "switch", deviceName, logChannel) {
  init(client, pin, deviceName, "", logChannel);
}

HAMqttServoGoAndBack::HAMqttServoGoAndBack(PubSubClient* client, uint8_t pin, String deviceName, String haDeviceName, String logChannel):HAMqttDeviceBase(client, "switch", deviceName, haDeviceName, logChannel) {
  init(client, pin, deviceName, haDeviceName, logChannel);
}

void HAMqttServoGoAndBack::init(PubSubClient* client, uint8_t pin, String deviceName, String haDeviceName, String logChannel) {
  _pin = pin;
  
  _deviceServoGoAndBackStatusChannel_to_server = _channelHeading + "/switch/" + _haDeviceName + "-" + _deviceName + "/switch/status";
  _deviceServoGoAndBackCommandChannel_from_server = _channelHeading + "/switch/" + _haDeviceName + "-" + _deviceName + "/switch/set";
    
    // _deviceAutoDiscoverData = "{\"dev_cla\":\"switch\",\"name\":\""+_haDeviceName + "-" + _deviceName+"\",\"state_topic\":\""+_deviceSwitchStatusChannel_to_server+"\",\"command_topic\":\""+_deviceSwitchCommandChannel_from_server+"\",\"payload_on\":\"on\",\"payload_off\":\"off\",\"optimistic\":false,\"unique_id\":\""+haDeviceName + "-" + _deviceName+"\"";
  _deviceAutoDiscoverData = "{\"dev_cla\":\"switch\",\"name\":\"" + _deviceName+"_"+_haDeviceName+"\",\"state_topic\":\""+_deviceServoGoAndBackStatusChannel_to_server+"\",\"command_topic\":\""+_deviceServoGoAndBackCommandChannel_from_server+"\",\"payload_on\":\"on\",\"payload_off\":\"off\",\"optimistic\":false,\"unique_id\":\""+_haDeviceName + "-" + _deviceName+"\"";
  if (!_haDeviceName.equals("")) {
    _deviceAutoDiscoverData = _deviceAutoDiscoverData + ",\"device\":{\"identifiers\":[\""+_haDeviceName+"\"],\"name\":\"" + _haDeviceName + "\"}";
  }
  _deviceAutoDiscoverData = _deviceAutoDiscoverData + "}";
  _stepTime = millis();
}
void HAMqttServoGoAndBack::callback(String topicString, String payloadString) {
  log(INFO, "get call back:topic:" + topicString + ":data:" + payloadString);
  if (topicString.equals(_deviceServoGoAndBackCommandChannel_from_server)) {
    if (payloadString.equals("on")) {
      _isOn = true;
      Serial.println("---------------get start command");
    }
    Serial.print("callback-switch-Set pin");
    Serial.println(_isOn?"ON":"OFF");
    publishServoGoBackStatus();
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
      processCallbackCommandCommon(topicString, payloadString);
    }
  }
}

void HAMqttServoGoAndBack::publishServoGoBackStatus() {
  _client->publish(_deviceServoGoAndBackStatusChannel_to_server.c_str(), _isOn?"on":"off");
  Serial.print("Publish status-switch:");
  Serial.println(_isOn?"on":"off");
  log(INFO, "Publish status-switch:" + _isOn?"on":"off");
}

void HAMqttServoGoAndBack::beforeReconnect() {
  log(ERROR, "[Disconnected] beforeDisconnect-Set pin to 0");
  Serial.println("afterDisconnect-Set pin to 0");
  digitalWrite(_pin, LOW);

}

void HAMqttServoGoAndBack::afterRconnected() {
  Serial.print("[Disconnected] afterReconnect-Set pin to ");
  Serial.println(_isOn?"ON":"OFF");
  log(ERROR, "afterReconnect-Set pin to " + _isOn?"ON":"OFF");  
  publishServoGoBackStatus();
}


void HAMqttServoGoAndBack::loop() {
  long current = millis();
  if ((current - _recordTime) > 60000 || (current - _recordTime) < 0) {
    publishDiscover();
    _recordTime = millis();
  }
  if (_isOn) {
    if (_isGo) {
      _servo->attach(_pin);
      _servo->write(_endAngle);
      _isGo = false;
      Serial.println("+++++++++++++++go");
      publishServoGoBackStatus();
      _stepTime = millis();
    } else {
      if((current - _stepTime) > 600 || (current - _stepTime) < 0) {
        _servo->write(_startAngle);
      }
      if ((current - _stepTime) > 1100 || (current - _stepTime) < 0) {
        _isGo = true;
        _isOn = false;
        Serial.println("+++++++++++++++back");
        publishServoGoBackStatus();
        _servo->detach();
        _stepTime = millis();
      }
    }
  }
  
}
void HAMqttServoGoAndBack::setup() {
  Serial.print("setup - Set pin to ");
  Serial.println(_isOn?"ON":"OFF");
  _servo = new Servo();
  _servo->attach(_pin);
  _servo->write(_startAngle);
  delay(1000);
  _servo->detach();
  Serial.println("Initialing:");
  Serial.print("default value:");
  Serial.println(_isOn);
  Serial.print("_deviceServoGoAndBackStatusChannel_to_server:");
  Serial.println(_deviceServoGoAndBackStatusChannel_to_server);
  Serial.print("_deviceServoGoAndBackCommandChannel_from_server:");
  Serial.println(_deviceServoGoAndBackCommandChannel_from_server);
  Serial.print("_deviceAutoDiscoverData:");
  Serial.println(_deviceAutoDiscoverData);
}

void HAMqttServoGoAndBack::doSubscribe() {
  Serial.print("Subscribing:");
  Serial.println(_deviceServoGoAndBackCommandChannel_from_server);
  Serial.print("Subscribing:");
  Serial.println(_deviceCommandChannel_from_server);
  _client->subscribe(_deviceServoGoAndBackCommandChannel_from_server.c_str());
  _client->subscribe(_deviceCommandChannel_from_server.c_str());
  log(INFO, "Subscribe:" + _deviceServoGoAndBackCommandChannel_from_server + ":" + _deviceCommandChannel_from_server);
}

bool HAMqttServoGoAndBack::isSwitchOn() {
  return _isOn;
}

void HAMqttServoGoAndBack::setDelayTime(int time) {
  _delayTime = time;
}

void HAMqttServoGoAndBack::setStartAngle(int angle) {
  _startAngle = angle;
}

void HAMqttServoGoAndBack::setEndAngle(int angle) {
  _endAngle = angle;
}
