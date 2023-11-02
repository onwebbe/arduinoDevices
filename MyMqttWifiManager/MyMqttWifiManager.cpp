#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <FS.h>
#include "MyMqttWifiManager.h"


MqttWifiManager::MqttWifiManager() {
  _wm = new WiFiManager(); 
}

MqttWifiManager::MqttWifiManager(String deviceName, String clientIP) {
  _device_name = deviceName;
  _client_ip = clientIP;
}

void MqttWifiManager::setDeviceName(String deviceName) {
  _device_name = deviceName;
}
void MqttWifiManager::setClientIP(String clientIP) {
  _client_ip = clientIP;
}
void MqttWifiManager::setClientId(String clientId) {
  _client_id = clientId;
}
void MqttWifiManager::setClientPassword(String clientPassword) {
  _client_password = clientPassword;
}
void MqttWifiManager::setClientPort(String port) {
  _client_port = port;
}
    
void MqttWifiManager::setup() {
  if (_startWifiCallback != NULL) {
    _startWifiCallback();
  }
  SPIFFS.begin();
  if (SPIFFS.exists(_networkFileName)) {
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
  if (_connectedCallback != NULL) {
    _connectedCallback(_wm->getWiFiSSID());
  }
}
void MqttWifiManager::reconnect() {
  // Loop until we're reconnected
  int i = 0;
  while(i < 5) {
    String networkStatus = _wm->getWLStatusString();
    if (networkStatus.equals("WL_CONNECTED")) {
      break;
    } else {
      if (_notConnectedCallBack != NULL) {
        _notConnectedCallBack();
      }
      Serial.print("failed, rc=");
      Serial.print(networkStatus);
      Serial.println(" try again in 2 mins");
      // Wait 2 min before retrying
      delay(120000);
    }
    i++;
  }
  if (i >= 5) {
    resetNetworkSettingsWhenFailed();
  }
  if (i > 0) {
    if (_connectedCallback != NULL) {
      _connectedCallback(_wm->getWiFiSSID());
    }
  }
}

void MqttWifiManager::resetNetworkSettingsWhenFailed() {
  Serial.println("reset network settings");
  delay(1000);
  _wm->resetSettings();
  delay(5000);
  Serial.println("restart");
  ESP.restart();
}
void MqttWifiManager::setNotConnectedCallback( std::function<void()> func ) {
  _notConnectedCallBack = func;
}
void MqttWifiManager::setConnectSuccessfulCallback( std::function<void(String)> func ) {
  _connectedCallback = func;
}
void MqttWifiManager::setStartWifiNetworkCallback( std::function<void()> func ) {
  _startWifiCallback = func;
}

String MqttWifiManager::getClientID() {
  return _client_id;
}
String MqttWifiManager::getClientPassword() {
  return _client_password;
}
String MqttWifiManager::getClientIP() {
  return _client_ip;
}
String MqttWifiManager::getClientPort() {
  return _client_port;
}

String MqttWifiManager::readFile(const char * path) {
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
void MqttWifiManager::saveConfigToFile() {
  String writeFileString = _client_id + "," + _client_password + "," + _client_ip + "," + _client_port;
  writeFile(_networkFileName.c_str(), writeFileString.c_str());
}
void MqttWifiManager::writeFile(const char * path, const char * message) {
  Serial.printf("Writing file: %s\ncontent:\n%s", path, message);

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

void MqttWifiManager::split(String zifuchuan,String fengefu,String result[]) {
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
