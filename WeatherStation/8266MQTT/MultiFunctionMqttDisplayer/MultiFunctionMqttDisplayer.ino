#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <FS.h>
#include <OneButton.h>
#include "Displayer.h"


#define DEVICE_NAME "MutiFunction-Displayer2"
#define CLIENT_ID "MutiFunction_Displayer2"
#define CLIENT_PASSWORD "multifunctionDisplayer123456"
#define BUILTIN_LED D2

#define WEATHER_CHANNEL "homedevice/sensor/WeatherStationSensor/data"
#define WEATHER_RAIN_CHANNEL "homedevice/binary_sensor/WeatherStationSensor/data"

#define YUGANG1_STATUS_CHANNEL "homedevice/sensor/AquariumLight-d003885a.aquariumStatus/text/status"
#define YUGANG1_COMMAND_CHANNEL "homedevice/yugang4/command"

#define YUGANG2_STATUS_CHANNEL "homedevice/sensor/AquariumLight-42c07296.aquariumStatus/text/status"
#define YUGANG2_COMMAND_CHANNEL "homedevice/yugang7/command"

#define YUGANG1_STATUS_LIGHT_CHANNEL "homedevice/light/AquariumLight-42c07296-light/brightness/status"
#define YUGANG1_STATUS_LIGHT_SWITCH_CHANNEL "homedevice/light/AquariumLight-42c07296-light/light/status"
#define YUGANG1_STATUS_PUMB_CHANNEL "homedevice/light/AquariumLight-42c07296-pumb/brightness/status"
#define YUGANG1_STATUS_HEAT_CHANNEL "homedevice/switch/AquariumLight-42c07296-heater/switch/status"
#define YUGANG1_STATUS_TEMP_CHANNEL "homedevice/sensor/AquariumLight-42c07296.aquariumTemperature/sensor/status"

#define YUGANG2_STATUS_LIGHT_CHANNEL "homedevice/light/yugang7/lightbrightness/status"
#define YUGANG2_STATUS_LIGHT_SWITCH_CHANNEL "homedevice/light/yugang7/light/switch"
#define YUGANG2_STATUS_PUMB_CHANNEL "homedevice/light/yugang7/pumbbrightness/status"
#define YUGANG2_STATUS_HEAT_CHANNEL "homedevice/switch/yugang7/heater/status"
#define YUGANG2_STATUS_TEMP_CHANNEL "homedevice/sensor/yugang7/temperature/status"
#define NEXT_PIN 13
#define PREV_PIN 12

OneButton next(NEXT_PIN, true);
OneButton prev(PREV_PIN, true);

WiFiManager wm;
bool shouldSaveConfig = false;

uint16_t MQTT_BROKER_PORT = 9013;
String mqtt_server("192.168.50.109");

WiFiClient espClient;
PubSubClient client(espClient);
Displayer displayer;
double weather_temperature;
double weather_humidity;
double weather_pressure;
double weather_wind_speed;
double weather_light;
String weather_isRain("false");

String yugang1_light;
String yugang1_pumb;
String yugang1_heat;
String yugang1_temp;

String yugang2_light;
String yugang2_pumb;
String yugang2_heat;
String yugang2_temp;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("start");
  displayer.init();
  displayer.displayConnecting();
  next.attachClick(clickNext);
  prev.attachClick(clickPrev);
  attachInterrupt(digitalPinToInterrupt(NEXT_PIN), moveNextTicks, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PREV_PIN), movePrevTicks, CHANGE);
  if (SPIFFS.exists("/network.txt")) {
    Serial.println("network setting file exists");
    String strRaw = readFile("/network.txt");
    String splited[2];
    Split(strRaw, ",", splited);
    String client_id = splited[0];
    String client_password = splited[1];
    String client_ip = splited[2];
    Serial.print("get setting file");
    Serial.print(client_id);
    Serial.println();
    Serial.print(client_password);
    Serial.println();
    Serial.print(client_ip);
    Serial.println();
    mqtt_server = client_ip;
  }
  SPIFFS.end();
  Serial.println("end start littleFS");
  setup_wifi();
  const char *clientIP = mqtt_server.c_str();
  Serial.println("connecting mqtt server");
  Serial.println(clientIP);
  Serial.println("mqtt port");
  Serial.println(MQTT_BROKER_PORT);
  client.setServer(clientIP, MQTT_BROKER_PORT);
  client.setCallback(callback);
  Serial.println("connected wifi");
  
  
  Serial.println("saved data initialized");
  Serial.println("init wifi done");
  
  displayer.displayWifiInfo();
  delay(3000);
}
long lastTime = millis();
long lastTime2 = millis();

void loop() {
  next.tick();
  prev.tick();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  if ((millis() - lastTime2) > 2000 || (millis() - lastTime2) < 0) {
    lastTime2 = millis();
    displayer.loop();
  }
  if ((millis() - lastTime) > 4000 || (millis() - lastTime) < 0) {
    Serial.println("Publish Status for YuGang");
    lastTime = millis();
    client.publish(YUGANG1_COMMAND_CHANNEL, "getStatus");
    client.publish(YUGANG2_COMMAND_CHANNEL, "getStatus");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String topicString(topic);
  char payloadChar[length];
  for(int i = 0; i < length; i++) {
    payloadChar[i] = (char) payload[i];
  }
  String payloadString(payloadChar);
  Serial.print("Payload length:");
  Serial.println(length);
  Serial.print("Payload data:");
  Serial.println(payloadChar);
  Serial.print("Message arrived [");
  Serial.print(topicString);
  Serial.print("] [");
  Serial.print(payloadString);
  Serial.println("]");
  if (topicString.equals(WEATHER_CHANNEL)) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payloadString);
    JsonObject obj = doc.as<JsonObject>();
    weather_temperature = obj[String("temperature")];
    weather_humidity = obj[String("humidity")];
    weather_pressure = obj[String("pressure")];
    weather_wind_speed = obj[String("wind_speed")];
    weather_light = obj[String("light")];
    displayer.setWeatherData(weather_temperature, weather_humidity, weather_light, weather_wind_speed, weather_pressure, weather_isRain.c_str());
//    {"temperature":16.34,"humidity":100.00,"pressure":1018.33,"wind_speed":0.00,"light":8}
  } else if (topicString.equals(WEATHER_RAIN_CHANNEL)) {
    weather_isRain = payloadString;
  } else if (topicString.equals(YUGANG1_STATUS_CHANNEL)) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payloadString);
    JsonObject obj = doc.as<JsonObject>();
    String lightSwitch = obj[String("lightswitch")];
    int lightBright = obj[String("lightBright")];
    String pumbSwitch = obj[String("pumbswitch")];
    int pumbBright = obj[String("pumbBright")];
    String heaterSwitch = obj[String("heaterSwitch")];
    double temperature = obj[String("temperature")];
    
    displayer.setYugangHeTunLight(lightBright);
    displayer.setYugangHeTunPump(pumbBright);
    displayer.setYugangHeTunHeater(heaterSwitch.equals("ON")?true:false);
    displayer.setYugangHeTunTemperature(temperature);
  } else if (topicString.equals(YUGANG2_STATUS_CHANNEL)) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payloadString);
    JsonObject obj = doc.as<JsonObject>();
    String lightSwitch = obj[String("lightswitch")];
    int lightBright = obj[String("lightBright")];
    String pumbSwitch = obj[String("pumbswitch")];
    int pumbBright = obj[String("pumbBright")];
    String heaterSwitch = obj[String("heaterSwitch")];
    double temperature = obj[String("temperature")];
    displayer.setYugangLeiLongLight(lightBright);
    displayer.setYugangLeiLongPump(pumbBright);
    displayer.setYugangLeiLongHeater(heaterSwitch.equals("ON")?true:false);
    displayer.setYugangLeiLongTemperature(temperature);
  }
  
  
//  else if (topicString.equals(YUGANG1_STATUS_LIGHT_CHANNEL)) {
//    displayer.setYugangHeTunLight(payloadString.toInt());
//  } else if (topicString.equals(YUGANG1_STATUS_LIGHT_SWITCH_CHANNEL)) {
//    if (payloadChar[1] = 'F') {
//      displayer.setYugangHeTunLight(0);
//    } else {
//      displayer.setYugangHeTunLight(255);
//    }
//  } else if (topicString.equals(YUGANG1_STATUS_PUMB_CHANNEL)) {
//    displayer.setYugangHeTunPump(payloadString.toInt());
//  } else if (topicString.equals(YUGANG1_STATUS_HEAT_CHANNEL)) {
//    if (payloadChar[1] = 'F') {
//      displayer.setYugangHeTunHeater(false);
//    } else {
//      displayer.setYugangHeTunHeater(true);
//    }
//  } else if (topicString.equals(YUGANG1_STATUS_TEMP_CHANNEL)) {
//    displayer.setYugangHeTunTemperature(payloadString.toDouble());
//  } else if (topicString.equals(YUGANG2_STATUS_LIGHT_CHANNEL)) {
//    displayer.setYugangLeiLongLight(payloadString.toInt());
//  } else if (topicString.equals(YUGANG2_STATUS_LIGHT_SWITCH_CHANNEL)) {
//    if (payloadChar[1] = 'F') {
//      displayer.setYugangLeiLongLight(0);
//    } else {
//      displayer.setYugangLeiLongLight(255);
//    }
//  } else if (topicString.equals(YUGANG2_STATUS_PUMB_CHANNEL)) {
//    displayer.setYugangLeiLongPump(payloadString.toInt());
//  } else if (topicString.equals(YUGANG2_STATUS_HEAT_CHANNEL)) {
//    if (payloadChar[1] = 'F') {
//      displayer.setYugangLeiLongHeater(false);
//    } else {
//      displayer.setYugangLeiLongHeater(true);
//    }
//  } else if (topicString.equals(YUGANG2_STATUS_TEMP_CHANNEL)) {
//    displayer.setYugangLeiLongTemperature(payloadString.toDouble());
//  }
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
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
//  WiFiManager wm;
  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  // wm.resetSettings();

  wm.setSaveConfigCallback(saveConfigCallback);

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result
  char clientid[] = CLIENT_ID;
  char clientpwd[] = CLIENT_PASSWORD;
  int clientIPLength = mqtt_server.length() + 1;
  char clientIP[clientIPLength];
  mqtt_server.toCharArray(clientIP, clientIPLength);
  WiFiManagerParameter custom_client_id("clientid", "Client Id", clientid, 50);
  WiFiManagerParameter custom_client_pwd("clientpwd", "Client Pass", CLIENT_PASSWORD, 100);
  WiFiManagerParameter custom_client_ip("clientip", "Client IP", clientIP, 100);
  wm.addParameter(&custom_client_id);
  wm.addParameter(&custom_client_pwd);
  wm.addParameter(&custom_client_ip);
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  String strDeviceName = DEVICE_NAME;
  String wifiName = strDeviceName;
  Serial.print("Creating wifi");
  Serial.print(wifiName);
  Serial.println();
  res = wm.autoConnect(wifiName.c_str(), ""); // password protected ap

  if(!res) {
      Serial.println("Failed to connect");
      delay(300000);
      ESP.restart();
  }
  Serial.println("connected...yeey :)");
  Serial.println(custom_client_id.getValue());
  Serial.println(custom_client_pwd.getValue());
  Serial.println(custom_client_ip.getValue());
  Serial.println("--------get wifi ssid--------");
  Serial.println(wm.getWiFiSSID());
  String client_id = custom_client_id.getValue();
  String client_password = custom_client_pwd.getValue();
  String client_ip = custom_client_ip.getValue();
  Serial.print("CLIENT_ID: ");
  Serial.print(client_id);
  Serial.print(":::::");
  Serial.println();
  Serial.print("CLIENT_PASSWORD: ");
  Serial.print(client_password);
  Serial.print(":::::");
  Serial.print("CLIENT_IP: ");
  Serial.print(client_ip);
  Serial.print(":::::");
  Serial.println();
  mqtt_server = client_ip;
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
    if (client.connect(clientId.c_str(), "", CLIENT_PASSWORD)) {
      Serial.println("connected");
      Serial.println("subscribing channels");
      // Once connected, publish an announcement...
      client.subscribe(WEATHER_CHANNEL);
      client.subscribe(WEATHER_RAIN_CHANNEL);

      client.subscribe(YUGANG1_STATUS_CHANNEL);
      client.subscribe(YUGANG2_STATUS_CHANNEL);
//      client.subscribe(YUGANG1_STATUS_LIGHT_CHANNEL);
//      client.subscribe(YUGANG1_STATUS_LIGHT_SWITCH_CHANNEL);
//      client.subscribe(YUGANG1_STATUS_PUMB_CHANNEL);
//      client.subscribe(YUGANG1_STATUS_HEAT_CHANNEL);
//      client.subscribe(YUGANG1_STATUS_TEMP_CHANNEL);
//      client.subscribe(YUGANG2_STATUS_LIGHT_CHANNEL);
//      client.subscribe(YUGANG2_STATUS_LIGHT_SWITCH_CHANNEL);
//      client.subscribe(YUGANG2_STATUS_PUMB_CHANNEL);
//      client.subscribe(YUGANG2_STATUS_HEAT_CHANNEL);
//      client.subscribe(YUGANG2_STATUS_TEMP_CHANNEL);
      
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
      writeFile("/network.txt", (strClientId + "," + strPwd + "," + mqtt_server).c_str());
      SPIFFS.end();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 mins");
      // Wait 5 seconds before retrying
      delay(120000);
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
ICACHE_RAM_ATTR void moveNextTicks() {
  // include all buttons here to be checked
  Serial.println("next terminate");
  //clickNext();
  next.tick(); // just call tick() to check the state.
}

ICACHE_RAM_ATTR void movePrevTicks() {
  Serial.println("prev terminate");
  //clickPrev();
  // include all buttons here to be checked
  prev.tick(); // just call tick() to check the state.
}
void clickNext() {
  Serial.println("movebextTick");
  displayer.nextPage();
  displayer.loop();
}
void clickPrev() {
  Serial.println("moveprevTick");
  displayer.prevPage();
  displayer.loop();
}
