#include "HAMqttDeviceBase.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "FS.h"

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

  _deviceStatusFileName = "/" + _type + "_" + _deviceName + ".txt";
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
void HAMqttDeviceBase::saveStatus() {
  Serial.print("START save status: ");
  Serial.println(_deviceStatusFileName);
  SPIFFS.begin();
  File file = SPIFFS.open(_deviceStatusFileName, "w");
  if(!file) {
    Serial.print(_deviceStatusFileName);
    Serial.println(" - failed to open file for writing");
    return;
  }
  String statusString = getStatusString();
  Serial.print("Get Status String for Save:");
  Serial.println(statusString);
  if(file.print(statusString)) {
    Serial.print(_deviceStatusFileName);
    Serial.println(" - file written");
  } else {
    Serial.print(_deviceStatusFileName);
    Serial.println(" - write failed");
  }
  file.close();
  delay(10);
  // SPIFFS.end();
  Serial.print("END save status: ");
  Serial.println(_deviceStatusFileName);
}
String HAMqttDeviceBase::getSavedStatus() {
  String statusString = "";
  Serial.print("START get status string - filename: ");
  Serial.println(_deviceStatusFileName);
  SPIFFS.begin();
  if (SPIFFS.exists(_deviceStatusFileName)){
    File file = SPIFFS.open(_deviceStatusFileName, "r");
    if(!file) {
      Serial.print(_deviceStatusFileName);
      Serial.println(" - failed to open file for reading");
      return "";
    }
    while(file.available()){
      int intData = file.read();
      statusString = statusString + String((char)intData);
    }
    Serial.print("Get data from file:");
    Serial.println(statusString);
    file.close();
    delay(10);
  } else {
    Serial.print(_deviceStatusFileName);
    Serial.println(" - file does not exists.");
  }
  
  // SPIFFS.end();
  Serial.print("END get get status string - filename: ");
  Serial.println(_deviceStatusFileName);
  Serial.print("Data from file:");
  Serial.println(statusString);
  return statusString;
}

void HAMqttDeviceBase::split(String zifuchuan,String fengefu,String result[]) {
  int weizhi; //找查的位置
  String temps;//临时字符串
  int i=0;
  do {
    weizhi = zifuchuan.indexOf(fengefu);//找到位置
    if(weizhi != -1) { //如果位置不为空
        temps=zifuchuan.substring(0,weizhi);//打印取第一个字符
        zifuchuan = zifuchuan.substring(weizhi+fengefu.length(), zifuchuan.length());
        //分隔后只取后面一段内容 以方便后面找查
    }
    else {  //上面实在找不到了就把最后的 一个分割值赋值出来以免遗漏
       if(zifuchuan.length() > 0)
        temps=zifuchuan;
    }
    result[i++]=temps;
    //Serial.println(result[i-1]);//在这里执行分割出来的字符下面不然又清空了
    temps="";
 }
 while(weizhi >= 0);
}
void HAMqttDeviceBase::restoreStatus() {
  String statusString = getSavedStatus();
  Serial.print("Get Saved Status: ");
  Serial.println(statusString);
  restoreStatus(statusString);
}

String HAMqttDeviceBase::getStatusString() {
  return "";
}

void HAMqttDeviceBase::restoreStatus(String statusString) {
}