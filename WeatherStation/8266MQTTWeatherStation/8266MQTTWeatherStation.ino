#include <ArduinoJson.h>
#include "WeatherDataRetriver.h"
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <FS.h>

#define DHT22PIN 16
#define HORNERPIN 17
#define RAINPIN 18

#define DEVICE_NAME "ESP8266Weather-530b"

#define MQTT_BROKER_HOST "onwebbe.gicp.net"
#define MQTT_BROKER_PORT 9013

#define TARGET_COMMAND_CHANNEL "homedevice/switch/ESP8266Weather-530b/set"
#define HEADBEAT_CHANNEL "homedevice/switch/ESP8266Weather-530b/state"
#define SEND_CHANNEL "homedevice/switch/ESP8266Weather-530b/set"
#define RECEIVE_CHANNEL "homedevice/switch/ESP8266Weather-530b/set"
#define DISCOVER_CHANNEL "homedevice/switch/ESP8266Weather-530b/config"

WeatherDataRetriver retriver = WeatherDataRetriver(DHT22PIN, HORNERPIN, RAINPIN);

WiFiManager wm;
boolean shouldSaveConfig = false;

String CLIENT_ID = "ESP8266Weather-530b";
String CLIENT_PASSWORD = "";
const char* mqtt_server = MQTT_BROKER_HOST;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("start");
  initSavedData();
  
  initWifi();

  client.setServer(mqtt_server, MQTT_BROKER_PORT);
  client.setCallback(callback);

  initWeather();
}
void resetNetworkSettingsWhenFailed() {
  Serial.println("reset network settings");
  delay(1000);
  wm.resetSettings();
  delay(5000);
  Serial.println("restart");
  ESP.restart();
}
void saveConfigCallback() {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}
void initWifi() {
  delay(10);
  Serial.println("start device done");
  Serial.println("start wifi");

  wm.setSaveConfigCallback(saveConfigCallback);
  
  char clientid[] = DEVICE_NAME;
  char *clientpwd;
  CLIENT_PASSWORD.toCharArray(clientpwd, CLIENT_PASSWORD.length());
  WiFiManagerParameter custom_client_id("clientid", "Client Id", clientid, 50);
  WiFiManagerParameter custom_client_pwd("clientpwd", "Client Pass", CLIENT_PASSWORD.c_str(), 100);
  wm.addParameter(&custom_client_id);
  wm.addParameter(&custom_client_pwd);
  bool res;
  
  String strDeviceName = DEVICE_NAME;
  String wifiName = "8266Light-" + strDeviceName;
  Serial.print("Creating wifi");
  Serial.print(wifiName);
  Serial.println();
  res = wm.autoConnect(wifiName.c_str(), ""); // password protected ap

  if(!res) {
      Serial.println("Failed to connect");
      resetNetworkSettingsWhenFailed();
  }
  Serial.println("connected...yeey :)");
  Serial.println(custom_client_id.getValue());
  Serial.println(custom_client_pwd.getValue());
  Serial.println("--------get wifi ssid--------");
  Serial.println(wm.getWiFiSSID());
  CLIENT_ID = custom_client_id.getValue();
  CLIENT_PASSWORD = custom_client_pwd.getValue();
  Serial.print("CLIENT_ID: ");
  Serial.print(CLIENT_ID);
  Serial.println();
  Serial.print("CLIENT_PASSWORD: ");
  Serial.print(CLIENT_PASSWORD);
  Serial.println();
}
void initWeather() {
  Serial.println("Start weather initialization");
  retriver.init();
}
String getWeatherDataJSON() {
  Serial.println("start get Weather JSON Data");
  String data = retriver.getWeatherData();
  Serial.println(data);
  return data;
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(5000);
  client.publish(TARGET_COMMAND_CHANNEL, getWeatherDataJSON().c_str());
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  client.publish(TARGET_COMMAND_CHANNEL, getWeatherDataJSON().c_str());
}
void initSavedData() {
  if (!SPIFFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }

  if (SPIFFS.exists("/network.txt")) {
    Serial.println("network setting file exists");
    String strRaw = readFile("/network.txt");
    String splited[2];
    Split(strRaw, ",", splited);
    CLIENT_ID = splited[0];
    CLIENT_PASSWORD = splited[1];
    Serial.print("get setting file");
    Serial.print(CLIENT_ID);
    Serial.println();
    Serial.print(CLIENT_PASSWORD);
    Serial.println();
  }
  SPIFFS.end();
  Serial.println("end start littleFS");
}

void reconnect() {
  // Loop until we're reconnected
  int i = 0;
  while (!client.connected()) {
    if (i >= 5) {
      resetNetworkSettingsWhenFailed();
    }
    Serial.print("Attempting MQTT connection...");
    Serial.print(i);
    // Create a random client ID
    String clientId = CLIENT_ID;
    // clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), "", CLIENT_PASSWORD.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      const char* publishData = getWeatherDataJSON().c_str();
      client.publish(DISCOVER_CHANNEL, publishData);
      // ... and resubscribe
      client.subscribe(RECEIVE_CHANNEL);
      digitalWrite(BUILTIN_LED, HIGH);
      
      String strClientId = CLIENT_ID;
      String strPwd = CLIENT_PASSWORD;
      if (!SPIFFS.begin()) {
        Serial.println("LittleFS mount failed");
        return;
      }
      SPIFFS.remove("/network.txt");
      Serial.print("Write to file");
      Serial.print("/network.txt with content:");
      Serial.print(strClientId + "," + strPwd);
      writeFile("/network.txt", (strClientId + "," + strPwd).c_str());
      SPIFFS.end();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    i = i + 1;
  }
}


String readFile(const char * path) {
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

void writeFile(const char * path, const char * message) {
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

void Split(String zifuchuan,String fengefu,String result[]) {
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
