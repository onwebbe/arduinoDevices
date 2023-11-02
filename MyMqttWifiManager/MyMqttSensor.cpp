#include "MyMqttSensor.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

MqttSensorConfig::MqttSensorConfig(String unit, String nodeName) {
  _unit = unit;
  _nodeName = nodeName;
}
String MqttSensorConfig::getUnit() {
  return _unit;
}
String MqttSensorConfig::getNodeName() {
  return _nodeName;
}
String MqttSensorConfig::getTemplate() {
  // return "{{ value_json." + _nodeName + " }}";
  return "";
}
String MqttSensorConfig::getData() {
  return _data;
}
void MqttSensorConfig::setData(String data){
  _data = data;
}

MqttSensor::MqttSensor(PubSubClient* client, uint8_t pin, String deviceName, String logChannel) {
  MqttSensor(client, pin, deviceName, "", logChannel);
}

MqttSensor::MqttSensor(PubSubClient* client, uint8_t pin, String deviceName, String haDeviceName, String logChannel) {
  _deviceName = deviceName;
  _pin = pin;
  _client = client;
  _haDeviceName = deviceName;
  
  _deviceSensorStatusChannel_to_server = _channelHeading + "/sensor/" + _deviceName + "/sensor/status";
  _deviceDataChannel_to_server =  _channelHeading + "/sensor/" + _deviceName + "/command/data";
  _deviceCommandChannel_from_server = _channelHeading + "/sensor/" + _deviceName + "/command/set";
  _deviceAutoDiscoverChannel_to_server = _channelHeading + "/sensor/" + _deviceName + "/config";
  
  _deviceLogChannel_to_server = logChannel;
  _recordTime = millis();
  _publishDataTime = millis();
  log(INFO, "Mqtt Sensor instance created");
  log(ERROR, "Publish command to " + _deviceCommandChannel_from_server + " for action");
  Serial.print("Publish command to ");
  Serial.print(_deviceCommandChannel_from_server);
  Serial.println(" for action");
}

void MqttSensor::callback(String topicString, String payloadString) {
  log(INFO, "get call back:topic:" + topicString + ":data:" + payloadString);
  if (topicString.equals(_deviceCommandChannel_from_server)) {
    StaticJsonDocument<768> doc;
    DeserializationError error = deserializeJson(doc, payloadString);
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.f_str());
      return;
    }
    String command = String(doc["cmd"]);
    String commandData = String(doc["data"]);
    if (command.equals("setDebugLevel")) {
      _debugLevel = commandData.toInt();
    }
  }
}

void MqttSensor::publishSensorStatus() {
  _client->publish(_deviceSensorStatusChannel_to_server.c_str(), 0);
  Serial.print("Publish status-sensor:");
  Serial.println(0);
  log(INFO, "Publish status-sensor:" + 0);
}

void MqttSensor::beforeReconnect() {
  log(ERROR, "[Disconnected] beforeDisconnect-Set pin to 0");
  Serial.println("afterDisconnect-Set pin to 0");
}

void MqttSensor::afterRconnected() {
  Serial.print("[Disconnected] afterReconnect-Set pin to ");
  log(ERROR, "afterReconnect-Set pin to " + _isOn?"ON":"OFF");
  publishSensorStatus();
}

void MqttSensor::log(int level, String s) {
  if (level > 0 && level <= _debugLevel) {
    String logString = "Sensor:" + _deviceName + ":" + s;
    Serial.print("Sending log:");
    Serial.print(_deviceLogChannel_to_server);
    Serial.print(":");
    Serial.println(logString);
    _client->publish(_deviceLogChannel_to_server.c_str(), logString.c_str());
  }
}

void MqttSensor::publishDiscover() {
  Serial.print("Sending discover string:");
  Serial.print(_deviceAutoDiscoverChannel_to_server);
  Serial.println(":");
  StaticJsonDocument<512> doc;
  doc["dev_cla"] = "sensor";
  doc["name"] = _deviceName;
  doc["state_topic"] = _deviceSensorStatusChannel_to_server;
  doc["unique_id"] = _deviceName;

  if (_haDeviceName.equals("")) {
    JsonObject device = doc.createNestedObject("device");
    device["identifiers"][0] = _deviceName;
    device["name"] = _haDeviceName;
  }
  
  for(int i = 0; i < _sensorDataConfigList->size(); i++) {
    MqttSensorConfig* configData = _sensorDataConfigList->get(i);
    doc["unit_of_measurement"] = configData->getUnit();
    doc["value_template"] = configData->getTemplate();
    String data;
    serializeJson(doc, data);
    log(INFO, "Sending discover channel:" + _deviceAutoDiscoverChannel_to_server + ":data:" + data);
    bool isSuccess = _client->publish(_deviceAutoDiscoverChannel_to_server.c_str(), data.c_str());
    log(INFO, "Sending discover:" + isSuccess?"success":"failed");
  }
}
void MqttSensor::addSensorConfig(MqttSensorConfig* configData) {
  _sensorDataConfigList->add(configData);
}
void MqttSensor::loop() {
  long current = millis();
  if ((current - _recordTime) > 60000 || (current - _recordTime) < 0) {
    publishDiscover();
    _recordTime = millis();
  }
  if ((current - _publishDataTime) > _publishGap || (current - _publishDataTime) < 0) {
    publishData();
    _recordTime = millis();
  }
  
}
void MqttSensor::publishData() {
  StaticJsonDocument<128> doc;
  for(int i = 0; i < _sensorDataConfigList->size(); i++) {
    MqttSensorConfig* configData = _sensorDataConfigList->get(i);
    doc[configData->getNodeName().c_str()] = configData->getData();
  }
  String data;
  serializeJson(doc, data);
  log(INFO, "Sending data channel:" + _deviceSensorStatusChannel_to_server + ":data:" + data);
  bool isSuccess = _client->publish(_deviceSensorStatusChannel_to_server.c_str(), data.c_str());
  log(INFO, "Sending data:" + isSuccess?"success":"failed");
}

void MqttSensor::setup() {
}

void MqttSensor::doSubscribe() {
  Serial.print("Subscribing:");
  Serial.println(_deviceCommandChannel_from_server);
  _client->subscribe(_deviceCommandChannel_from_server.c_str());
  log(INFO, "Subscribe:" + _deviceCommandChannel_from_server);
}
void MqttSensor::setDebugLevel(int debugLevel) {
  _debugLevel = debugLevel;
}
