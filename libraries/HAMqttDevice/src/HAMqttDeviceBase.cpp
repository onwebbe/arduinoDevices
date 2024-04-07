#include "HAMqttDeviceBase.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

HAMqttDeviceBase::HAMqttDeviceBase(PubSubClient* client, String type, String deviceName, String logChannel) {
  HAMqttDeviceBase(client, type, deviceName, "", logChannel);
}

HAMqttDeviceBase::HAMqttDeviceBase(PubSubClient* client, String type,  String deviceName, String haDeviceName, String logChannel) {
  _deviceName = deviceName;
  _client = client;
  _haDeviceName = haDeviceName;
  _type = type;

  _deviceDataChannel_to_server =  _channelHeading + "/" + type + "/" + haDeviceName + "-" + _deviceName + "/command/data";
  _deviceCommandChannel_from_server = _channelHeading + "/" + type + "/" + haDeviceName + "-" + _deviceName + "/command/set";
  _deviceAutoDiscoverChannel_to_server = _channelHeading + "/" + type + "/" +haDeviceName + "-" +  _deviceName + "/config";
  _deviceAutoDiscoverData = _deviceAutoDiscoverData;
  _deviceLogChannel_to_server = logChannel;
  _recordTime = millis();
  log(INFO, "Mqtt " + type + " instance created");
  log(ERROR, "Publish command to " + _deviceCommandChannel_from_server + " for action");
  Serial.print("Publish command to ");
  Serial.print(_deviceCommandChannel_from_server);
  Serial.println(" for action");
}
void HAMqttDeviceBase::setHeading(String heading) {
  _channelHeading = heading;
}
void HAMqttDeviceBase::processCallbackCommandCommon(String topicString, String payloadString) {
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
  }
}
void HAMqttDeviceBase::callback(String topicString, String payloadString) {
  log(INFO, "get call back:topic:" + topicString + ":data:" + payloadString);
  if (topicString.equals(_deviceCommandChannel_from_server)) {
    if (payloadString.equals("getStatus")) {
    } else {
      processCallbackCommandCommon(topicString, payloadString);
    }
  }
}


void HAMqttDeviceBase::beforeReconnect() {
  log(ERROR, "[Disconnected] beforeDisconnect");
  Serial.println("afterDisconnect-Set pin to 0");
}

void HAMqttDeviceBase::afterRconnected() {
  log(ERROR, "[Disconnected] afterReconnect");
  Serial.print("[Disconnected] afterReconnect");
}

void HAMqttDeviceBase::log(int level, String s) {
  String levelString = "ERROR";
  switch(level) {
    case 1: levelString = "ERROR";break;
    case 2: levelString = "INFO";break;
    case 3: levelString = "DEBUG";break;
  }
  String logString = "LIGHT:" + _deviceName + ":" + levelString + ":" + s;
  Serial.println(logString);
  if (level > 0 && level <= _debugLevel) {
    Serial.print("----Sending log----");
    Serial.print(_deviceLogChannel_to_server);
    Serial.print(":");
    Serial.println(logString);
    _client->publish(_deviceLogChannel_to_server.c_str(), logString.c_str());
  }
}

void HAMqttDeviceBase::publishDiscover() {
  Serial.print("Sending discover string:");
  Serial.print(_deviceAutoDiscoverChannel_to_server);
  Serial.print(":");
  Serial.println(_deviceAutoDiscoverData);
  log(INFO, "Sending discover channel:" + _deviceAutoDiscoverChannel_to_server + ":data:" + _deviceAutoDiscoverData);
  bool isSuccess = _client->publish(_deviceAutoDiscoverChannel_to_server.c_str(), _deviceAutoDiscoverData.c_str());
  log(INFO, "Sending discover:" + isSuccess?"success":"failed");
}

void HAMqttDeviceBase::loop() {
  long current = millis();
  if ((current - _recordTime) > _discoverTimeGap || (current - _recordTime) < 0) {
    publishDiscover();
    _recordTime = millis();
  }
}
void HAMqttDeviceBase::setup() {
  log(INFO, "Setup");
}

void HAMqttDeviceBase::doSubscribe() {
  // log(INFO, "Do Subscribe");
  // Serial.print("Subscribing:");
  // Serial.println(_deviceSwitchCommandChannel_from_server);
  // Serial.print("Subscribing:");
  // Serial.println(_deviceBrightCommandChannel_from_server);
  // Serial.print("Subscribing:");
  // Serial.println(_deviceCommandChannel_from_server);
  // _client->subscribe(_deviceSwitchCommandChannel_from_server.c_str());
  // _client->subscribe(_deviceBrightCommandChannel_from_server.c_str());
  // _client->subscribe(_deviceCommandChannel_from_server.c_str());
  // log(INFO, "Subscribe:" + _deviceSwitchCommandChannel_from_server + ":" + _deviceBrightCommandChannel_from_server + ":" + _deviceCommandChannel_from_server);
}
void HAMqttDeviceBase::setDiscoverTimeGap(long gap) {
  _discoverTimeGap = gap;
}
void HAMqttDeviceBase::setDebugLevel(int debugLevel) {
  _debugLevel = debugLevel;
}
