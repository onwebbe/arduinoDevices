#include "HAMqttSensor.h"
#include "HAMqttDeviceBase.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

HAMqttSensorConfig::HAMqttSensorConfig(String unit, String nodeName) {
  _unit = unit;
  _nodeName = nodeName;
  _devClass = nodeName;
}
String HAMqttSensorConfig::getUnit() {
  return _unit;
}
String HAMqttSensorConfig::getNodeName() {
  return _nodeName;
}
String HAMqttSensorConfig::getTemplate() {
  return "{{ value_json." + _nodeName + " }}";
}
String HAMqttSensorConfig::getData() {
  return _data;
}
void HAMqttSensorConfig::setData(String data) {
  _data = data;
}
void HAMqttSensorConfig::setType(int type) {
  _type = type;
}
int HAMqttSensorConfig::getType() {
  return _type;
}
String HAMqttSensorConfig::getDevClass() {
  return _devClass;
}
void HAMqttSensorConfig::setDevClass(String devClass) {
  _devClass = devClass;
}
HAMqttSensor::HAMqttSensor(PubSubClient* client, String deviceName, String logChannel) : HAMqttDeviceBase(client, "sensor", deviceName, logChannel) {
  HAMqttSensor(client, deviceName, "", logChannel);
}

HAMqttSensor::HAMqttSensor(PubSubClient* client, String deviceName, String haDeviceName, String logChannel) : HAMqttDeviceBase(client, "sensor", deviceName, haDeviceName, logChannel) {
  _deviceSensorStatusChannel_to_server = _channelHeading + "/sensor/" + haDeviceName + "." + _deviceName + "/sensor/status";
}

void HAMqttSensor::callback(String topicString, String payloadString) {
  log(INFO, "get call back:topic:" + topicString + ":data:" + payloadString);
  if (topicString.equals(_deviceCommandChannel_from_server)) {
    if (payloadString.equals("getStatus")) {
      
    } else {
      processCallbackCommandCommon(topicString, payloadString);
    }
  }
}

void HAMqttSensor::beforeReconnect() {
  log(ERROR, "[Disconnected] beforeDisconnect");
  Serial.println("afterDisconnect-Set pin to 0");
}

void HAMqttSensor::afterRconnected() {
  Serial.print("[Disconnected] afterReconnect");
  publishStatus();
}

void HAMqttSensor::publishDiscover() {
  for (int i = 0; i < _sensorDataConfigList->size(); i++) {
    HAMqttSensorConfig* configData = _sensorDataConfigList->get(i);

    StaticJsonDocument<512> doc;
    doc["dev_cla"] = configData->getDevClass();
    // doc["name"] = _haDeviceName + "." + _deviceName + "_" + configData->getNodeName();
    doc["name"] = _deviceName + "_" + _haDeviceName + "_" + configData->getNodeName();
    doc["state_topic"] = _deviceSensorStatusChannel_to_server;
    if (!_haDeviceName.equals("")) {
      JsonObject device = doc.createNestedObject("device");
      device["identifiers"][0] = _haDeviceName;
      device["name"] = _haDeviceName;
    }
    
    doc["unit_of_measurement"] = configData->getUnit();
    doc["value_template"] = configData->getTemplate();
    doc["unique_id"] = _haDeviceName + "_" + _deviceName + "_" + configData->getNodeName();
    serializeJson(doc, _deviceAutoDiscoverData);
    Serial.println(_deviceAutoDiscoverData);
    Serial.print("Sending discover string:");
    _deviceAutoDiscoverChannel_to_server = _channelHeading + "/sensor/" +_haDeviceName + "_" + _deviceName + "_" + configData->getNodeName() + "/config";
    Serial.print(_deviceAutoDiscoverChannel_to_server);
    Serial.println(":");
    log(INFO, "Sending discover channel:" + _deviceAutoDiscoverChannel_to_server + ":data:" + _deviceAutoDiscoverData);
    bool isSuccess = _client->publish(_deviceAutoDiscoverChannel_to_server.c_str(), _deviceAutoDiscoverData.c_str());
    log(INFO, "Sending discover:" + isSuccess ? "success" : "failed");
  }
}
void HAMqttSensor::addSensorConfig(HAMqttSensorConfig* configData) {
  _sensorDataConfigList->add(configData);
}
void HAMqttSensor::loop() {
  long current = millis();
  if ((current - _recordTime) > _discoverTimeGap || (current - _recordTime) < 0) {
    publishDiscover();
    _recordTime = millis();
  }
  if ((current - _publishDataTime) > _publishGap || (current - _publishDataTime) < 0) {
    Serial.println("Sensor Loop");
    publishStatus();
    _publishDataTime = millis();
  }
}
void HAMqttSensor::publishData() {
  StaticJsonDocument<128> doc;
  for (int i = 0; i < _sensorDataConfigList->size(); i++) {
    HAMqttSensorConfig* configData = _sensorDataConfigList->get(i);
    if (configData->getType() == 2) {
      doc[configData->getNodeName()] = configData->getData().toDouble();
    } else {
      doc[configData->getNodeName()] = configData->getData();
    }
    
  }
  String data;
  serializeJson(doc, data);
  Serial.print("Sending data channel:");
  Serial.print(_deviceSensorStatusChannel_to_server);
  Serial.print(":data:");
  Serial.println(data);
  log(INFO, "Sending data channel:" + _deviceSensorStatusChannel_to_server + ":data:" + data);
  

  bool isSuccess = _client->publish(_deviceSensorStatusChannel_to_server.c_str(), data.c_str());
  log(INFO, "Sending data:" + isSuccess ? "success" : "failed");
}

void HAMqttSensor::setup() {
}

void HAMqttSensor::doSubscribe() {
  Serial.print("Subscribing:");
  Serial.println(_deviceCommandChannel_from_server);
  _client->subscribe(_deviceCommandChannel_from_server.c_str());
  log(INFO, "Subscribe:" + _deviceCommandChannel_from_server);
}

void HAMqttSensor::setPublishGap(int time) {
  _publishGap = time;
}

void HAMqttSensor::restoreStatus() {
  Serial.println("Sensor Resore status");
  publishStatus();
}

void HAMqttSensor::publishStatus() {
  if (_callToSetData != NULL) {
    _callToSetData(this);
  }
  publishData();
}

void HAMqttSensor::setSensorData(String nodeName, String data) {
  for (int i = 0; i < _sensorDataConfigList->size(); i++) {
    HAMqttSensorConfig* conf = _sensorDataConfigList->get(i);
    if (conf->getNodeName().equals(nodeName)) {
      conf->setData(data);
      return;
    }
  }
}

void HAMqttSensor::callToSetData(std::function<void(HAMqttSensor*)> func) {
  _callToSetData = func;
}