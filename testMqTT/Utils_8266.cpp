#include <FS.h>
#include "Utils_8266.h"
WiFiManager* Utils_8266::_wifiManager;
String Utils_8266::DISCOVER_HEAD = "homedevice";
String Utils_8266::_t;
String Utils_8266::_h;
String Utils_8266::_l;
String Utils_8266::_p;
String Utils_8266::_w;
String Utils_8266::_r;
void Utils_8266::setupWiFiManager(WiFiManager* wifiManager) {
  Utils_8266::_wifiManager = wifiManager;
}
void Utils_8266::resetNetworkSettingsWhenFailed() {
  Serial.println("reset network settings");
  delay(1000);
  Utils_8266::_wifiManager->resetSettings();
  delay(5000);
  Serial.println("restart");
  ESP.restart();
}

void Utils_8266::restartOnly() {
  delay(1000);
  Serial.println("restart");
  ESP.restart();
}
String Utils_8266::readFile(const char * path) {
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
      result = result + (char)intData;
    }
  }
  file.close();
  return result;
}

void Utils_8266::writeFile(const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

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
}

void Utils_8266::Split(String zifuchuan,String fengefu,String result[]) {
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
