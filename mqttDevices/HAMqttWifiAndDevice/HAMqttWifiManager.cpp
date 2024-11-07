#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <FS.h>
#include "HAMqttWifiManager.h"
#include <PubSubClient.h>

HAMqttWifiManager::HAMqttWifiManager() {
  _wm = new WiFiManager(); 
}

HAMqttWifiManager::HAMqttWifiManager(String deviceName, String clientIP) {
  _device_name = deviceName;
  _client_ip = clientIP;
}

void HAMqttWifiManager::setDeviceName(String deviceName) {
  _device_name = deviceName;
}
void HAMqttWifiManager::setClientIP(String clientIP) {
  _client_ip = clientIP;
}
void HAMqttWifiManager::setClientId(String clientId) {
  _client_id = clientId;
}
void HAMqttWifiManager::setClientPassword(String clientPassword) {
  _client_password = clientPassword;
}
void HAMqttWifiManager::setClientPort(String port) {
  _client_port = port;
}
void HAMqttWifiManager::init(String ssid, String password, String ip, String port) {
  _client_id = ssid;
  _client_password = password;
  _client_ip = ip;
  _client_port = port;
}
bool HAMqttWifiManager::loop() {
  reconnect();
  _pubClient->loop();
  return true;
}
bool HAMqttWifiManager::isNetworkConnected() {
  String networkStatus = _wm->getWLStatusString();
  return networkStatus.equals("WL_CONNECTED");
}
bool HAMqttWifiManager::initFromFile() {
  SPIFFS.begin();
  bool hasFile = false;
  if (SPIFFS.exists(_networkFileName)) {
    hasFile = true;
    Serial.println("network setting file exists");
    String strRaw = readFile(_networkFileName.c_str());
    String splited[4];
    split(strRaw, ",", splited);
    _client_id = splited[0];
    _client_password = splited[1];
    _client_ip = splited[2];
    _client_port = splited[3];
    Serial.print("get setting file:");
    Serial.print(_client_id);
    Serial.print("---");
    Serial.print(_client_password);
    Serial.print("---");
    Serial.print(_client_ip);
    Serial.print("---");
    Serial.println(_client_port);
  }
  Serial.println("end start littleFS");

  delay(10);
  // SPIFFS.end();
  return hasFile;
} 
void HAMqttWifiManager::setup() {
  bool fileExists = initFromFile();
  if (_startWifiCallback != NULL) {
    _startWifiCallback();
  }
  
  // We start by connecting to a WiFi network
  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
//  WiFiManager wm;
  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  // wm.resetSettings();

  // wm->setSaveConfigCallback(saveConfigCallback);

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result
  const char* clientid = _client_id.c_str();
  const char* clientpwd = _client_password.c_str();
  const char* clientIP = _client_ip.c_str();
  const char* port = _client_port.c_str();
  Serial.print("prepare setting to wfmanager:");
  Serial.print(clientid);
  Serial.print("---");
  Serial.print(clientpwd);
  Serial.print("---");
  Serial.print(clientIP);
  Serial.print("---");
  Serial.print(port);
  WiFiManagerParameter custom_client_id("clientid", "Client Id", clientid, 50);
  WiFiManagerParameter custom_client_pwd("clientpwd", "Client Pass", clientpwd, 100);
  WiFiManagerParameter custom_client_ip("clientip", "Client IP", clientIP, 100);
  WiFiManagerParameter custom_client_port("clientport", "Client Port", port, 5);
  _wm->addParameter(&custom_client_id);
  _wm->addParameter(&custom_client_pwd);
  _wm->addParameter(&custom_client_ip);
  _wm->addParameter(&custom_client_port);
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  String strDeviceName = _device_name;
  String wifiName = _device_name;
  Serial.print("Creating wifi ");
  Serial.print(wifiName);
  Serial.println();
  res = _wm->autoConnect(wifiName.c_str(), ""); // password protected ap

  if(!res) {
      Serial.println("Failed to connect");
      delay(300000);
      ESP.restart();
  }
  Serial.println("connected...yeey :)");
  _client_id = custom_client_id.getValue();
  _client_password = custom_client_pwd.getValue();
  _client_ip = custom_client_ip.getValue();
  String clientPortTmp = custom_client_port.getValue();
  if (clientPortTmp != NULL && clientPortTmp.equals("")) {
    clientPortTmp = "9013";
  }
  _client_port = clientPortTmp;

  Serial.print("CLIENT_ID: ");
  Serial.print(_client_id);
  Serial.print(":::::");
  Serial.print("CLIENT_PASSWORD: ");
  Serial.print(_client_password);
  Serial.print(":::::");
  Serial.print("CLIENT_IP: ");
  Serial.print(_client_ip);
  Serial.print("CLIENT_PORT: ");
  Serial.print(_client_port);
  Serial.print("--------get wifi ssid--------");
  Serial.println(_wm->getWiFiSSID());
  reconnect();
  // do reconnection
  saveConfigToFile();
}
void HAMqttWifiManager::reconnect(int retryCount) {
  // Loop until we're reconnected
  int i = 0;
  int realRetryCount = retryCount > 0 ? retryCount : 5;
  while(i < realRetryCount) {
    String networkStatus = _wm->getWLStatusString();
    bool isPubClientConnected = _pubClient->connected();
    if (networkStatus.equals("WL_CONNECTED") && isPubClientConnected == true) {
      
      return;
    } else {
      Serial.printf("mqttServerIP:%s port:%d", _client_ip.c_str(), _client_port.toInt());
      if (_beforeNotConnectedCallBack != NULL) {
        _beforeNotConnectedCallBack();
      }
      _pubClient->setServer(_client_ip.c_str(), _client_port.toInt());
      if (_mqttRecieveMessageCallback != NULL) {
        _pubClient->setCallback(_mqttRecieveMessageCallback);
      }
      if (_pubClient->connect(_client_id.c_str(), "", _client_password.c_str())) {
        Serial.println("Connected to MQTT");
        if (_connectedCallback != NULL) {
          _connectedCallback(_wm->getWiFiSSID());
        }
        return;
      }
      Serial.println("Connect to mqtt server failed, retrying");


      if (_notConnectedCallBack != NULL) {
        _notConnectedCallBack();
      }
      Serial.print("wifi and mqtt network connection failed, rc=");
      Serial.print(networkStatus);
      Serial.println(" try again in 2 mins");
      // Wait 2 min before retrying
      delay(120000);
    }
    i++;
  }
  if (i >= realRetryCount) {
    if (retryCount > 0) {
      resetNetworkSettingsWhenFailed();
    } else {
      ESP.restart();
    }
  } else { // may not be called because if connected will go break directly
    if (_connectedCallback != NULL) {
      _connectedCallback(_wm->getWiFiSSID());
    }
    return;
  }
  // if (i > 0) {
  //   if (_connectedCallback != NULL) {
  //     _connectedCallback(_wm->getWiFiSSID());
  //   }
  // }
}
void HAMqttWifiManager::reconnect() {
  reconnect(0);
}

void HAMqttWifiManager::resetNetworkSettingsWhenFailed() {
  Serial.println("reset network settings");
  delay(1000);
  _wm->resetSettings();
  delay(5000);
  Serial.println("restart");
  ESP.restart();
}
void HAMqttWifiManager::setNotConnectedCallback( std::function<void()> func ) {
  _notConnectedCallBack = func;
}
void HAMqttWifiManager::setConnectSuccessfulCallback( std::function<void(String)> func ) {
  _connectedCallback = func;
}
void HAMqttWifiManager::setStartWifiNetworkCallback( std::function<void()> func ) {
  _startWifiCallback = func;
}

String HAMqttWifiManager::getClientID() {
  return _client_id;
}
String HAMqttWifiManager::getClientPassword() {
  return _client_password;
}
String HAMqttWifiManager::getClientIP() {
  return _client_ip;
}
String HAMqttWifiManager::getClientPort() {
  return _client_port;
}

String HAMqttWifiManager::readFile(const char * path) {
  Serial.printf("Reading file: %s\n", path);

  File file = SPIFFS.open(path, "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return "";
  }

  Serial.print("Read from file: ");
  Serial.print(file.size());
  String result;
  if (SPIFFS.exists(path)) {
    for(int i = 0; i < file.size(); i++) {
      int intData = file.read();
      Serial.write(intData);
      result = result + String((char)intData);
    }
  }
  
  file.close();
  Serial.print("file content: ");
  Serial.println(result);
  return result;
}
void HAMqttWifiManager::saveConfigToFile() {
  String writeFileString = _client_id + "," + _client_password + "," + _client_ip + "," + _client_port;
  writeFile(_networkFileName.c_str(), writeFileString.c_str());
}
void HAMqttWifiManager::writeFile(const char * path, const char * message) {
  Serial.printf("Writing file: %s\ncontent:\n%s", path, message);
  SPIFFS.begin();
  File file = SPIFFS.open(path, "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  delay(2000); // Make sure the CREATE and LASTWRITE times are different
  file.close();
  SPIFFS.end();
}

void HAMqttWifiManager::split(String zifuchuan,String fengefu,String result[]) {
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

void HAMqttWifiManager::setupMqttPubClient(PubSubClient* client, std::function<void(char*, byte*, unsigned int)> callback) {
  _pubClient = client;
  _mqttRecieveMessageCallback = callback;
  _pubClient->setCallback(callback);
}

void HAMqttWifiManager::setBeforeNotConnectedCallback( std::function<void()> func ) {
  _beforeNotConnectedCallBack = func;
}