

/*
 Basic ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <OneButton.h>
#include <FS.h>

#define DEVICE_NAME "light_yugang7"

#define LIGHT_STATE_CHANNEL "homedevice/light/yugang7/light/status"
#define LIGHT_COMMAND_CHANNEL "homedevice/light/yugang7/light/switch"
#define LIGHT_BRIGHT_STATE_CHANNEL "homedevice/light/yugang7/lightbrightness/status"
#define LIGHT_BRIGHT_COMMAND_CHANNEL "homedevice/light/yugang7/lightbrightness/set"

#define PUMB_STATE_CHANNEL "homedevice/light/yugang7/pumb/status"
#define PUMB_COMMAND_CHANNEL "homedevice/light/yugang7/pumb/switch"
#define PUMB_BRIGHT_STATE_CHANNEL "homedevice/light/yugang7/pumbbrightness/status"
#define PUMB_BRIGHT_COMMAND_CHANNEL "homedevice/light/yugang7/pumbbrightness/set"
      
#define HEATER_STATE_CHANNEL "homedevice/switch/yugang7/heater/status"
#define HEATER_COMMAND_CHANNEL "homedevice/switch/yugang7/heater/switch"

#define DISCOVER_CHANNEL "homedevice/light/yugang7/config"
#define STATE_CHANNEL "homedevice/light/yugang7/status"

#define TEMP_CHANNEL "homedevice/sensor/yugang7/temperature/status"

#define TARGET_COMMAND_SET_LOW_CHANNEL "homedevice/yugang7/light/setLow"
#define TARGET_COMMAND_SET_HIGH_CHANNEL "homedevice/yugang7/light/setHigh"

#define DEVICE_COMMAND "homedevice/yugang7/command"
#define DEVICE_STATUS "homedevice/yugang7/status"

//#define CLIENT_ID "ESP8266Client-522b"
//#define CLIENT_PASSWORD "autogenerated_token_522b_7fb50d48-342a-11ed-a261-0242ac120002"
#define MQTT_BROKER_HOST "192.168.50.109"
//#define MQTT_BROKER_HOST "onwebbe.gicp.net"
#define MQTT_BROKER_PORT 9013

// #define WIFI_NAME "HUAWEI—7539"
#define WIFI_NAME "pankey_asus"
#define WIFI_PASSWORD "1234554321"
#define LIGHT_PIN 13
#define HEATER_PIN 12
#define PUMB_PIN 14
#define RESET_NETWORK_BUTTON_PIN 16
#define SWITCH_BUTTON_PIN 5

#define CLIENT_ID "HomeAssistantSmart_yugang_7"
#define  CLIENT_PASSWORD "smartyugang_7"
// Update these with values suitable for your network.
const int TEMPERATURE_PIN = A0;
const char* ssid = WIFI_NAME;
const char* password = WIFI_PASSWORD;
String mqtt_server("192.168.50.109");
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastHeartBeat = 0;
unsigned long lastStatusUpate = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
boolean shouldSaveConfig = false;

int tempHigh = 30;
int tempLow = 26;
int brightnessValue = 100;
int pumbbrightnessValue = 140;
WiFiManager wm;
OneButton resetNetworkButton(RESET_NETWORK_BUTTON_PIN, true);
OneButton switchButton(SWITCH_BUTTON_PIN, true);
boolean isOn = false;
boolean isPumbOn = false;
boolean isHeaterOn = false;
      
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
  String wifiName = "SmartYugang-" + strDeviceName;
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
  mqtt_server = MQTT_BROKER_HOST;
}

void callback(char* topic, byte* payload, unsigned int length) {
  
  String topicString(topic);
  String setLowString(TARGET_COMMAND_SET_LOW_CHANNEL);
  String setHighString(TARGET_COMMAND_SET_HIGH_CHANNEL);
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
  if (topicString.equals(setLowString)) {
    tempLow = payloadString.toInt();
  } else if (topicString.equals(setHighString)) {
    tempHigh = payloadString.toInt();
  } else if (topicString.equals(LIGHT_COMMAND_CHANNEL)) {
    Serial.println("Light switch received");
    if (payloadString.equals("off")) {
      Serial.println("Switch OFF");
      digitalWrite(LIGHT_PIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
      digitalWrite(BUILTIN_LED, LOW);
      // client.publish(STATE_CHANNEL, "off");
      isOn = false;
    } else {
      Serial.println("Switch ON");
      // digitalWrite(LIGHT_PIN, HIGH);  // Turn the LED off by making the voltage HIGH
      analogWrite(LIGHT_PIN, brightnessValue);
      digitalWrite(BUILTIN_LED, HIGH);
      // client.publish(STATE_CHANNEL, "on");
      isOn = true;
    }
    client.publish(LIGHT_STATE_CHANNEL, isOn?"on":"off");
  } else if (topicString.equals(LIGHT_BRIGHT_COMMAND_CHANNEL)) {
    brightnessValue = payloadString.toInt();
    analogWrite(LIGHT_PIN, brightnessValue);
    
    String brightValueStr(brightnessValue);
    char brightValueStrChar[brightValueStr.length()];
    brightValueStr.toCharArray(brightValueStrChar, brightValueStr.length() + 1);
    client.publish(LIGHT_BRIGHT_STATE_CHANNEL, brightValueStrChar);
    Serial.print("Publish light brightness:[");
    Serial.print(LIGHT_BRIGHT_STATE_CHANNEL);
    Serial.print("] data:[");
    Serial.print(brightValueStrChar);
    Serial.println("]");
  } else if (topicString.equals(PUMB_COMMAND_CHANNEL)) {
    if (payloadString.equals("off")) {
      Serial.println("PUMB OFF");
      digitalWrite(PUMB_PIN, LOW);
      isPumbOn = false;
    } else {
      Serial.println("PUMB ON");
      analogWrite(PUMB_PIN, 255);
      delay(1000);
      analogWrite(PUMB_PIN, pumbbrightnessValue);
      isPumbOn = true;
    }
    client.publish(PUMB_STATE_CHANNEL, isPumbOn?"on":"off");
    String pumbValueStr(pumbbrightnessValue);
    char pumbValueChar[pumbValueStr.length()];
    pumbValueStr.toCharArray(pumbValueChar, pumbValueStr.length() + 1);
    client.publish(PUMB_BRIGHT_STATE_CHANNEL, pumbValueChar);
    
  } else if (topicString.equals(PUMB_BRIGHT_COMMAND_CHANNEL)) {
    pumbbrightnessValue = payloadString.toInt();
    analogWrite(PUMB_PIN, 255);
    delay(1000);
    analogWrite(PUMB_PIN, pumbbrightnessValue);
    Serial.print("PUMB Adjuct:");
    Serial.println(pumbbrightnessValue);

    String pumbValueStr(pumbbrightnessValue);
    char pumbValueChar[pumbValueStr.length()];
    pumbValueStr.toCharArray(pumbValueChar, pumbValueStr.length() + 1);
    client.publish(PUMB_BRIGHT_STATE_CHANNEL, pumbValueChar);
    Serial.print("Publish pumb brightness:[");
    Serial.print(PUMB_BRIGHT_STATE_CHANNEL);
    Serial.print("] data:[");
    Serial.print(pumbValueChar);
    Serial.println("]");
    
  } else if (topicString.equals(HEATER_COMMAND_CHANNEL)) {
    if (payloadString.equals("off")) {
      Serial.println("HEATER OFF");
      digitalWrite(HEATER_PIN, LOW);
      isHeaterOn = false;
    } else {
      Serial.println("HEATER ON");
      digitalWrite(HEATER_PIN, HIGH);
      isHeaterOn = true;
    }
    client.publish(HEATER_STATE_CHANNEL, isHeaterOn?"on":"off");
  } else if (topicString.equals(DEVICE_COMMAND)) {
    if (payloadString.equals("getStatus")) {
      Serial.println("Get Status Command, will update status");
      StaticJsonDocument<96> doc;
      doc["lightswitch"] = isOn?"ON":"OFF";
      doc["lightBright"] = brightnessValue;
      doc["pumbswitch"] = isPumbOn?"ON":"OFF";;
      doc["pumbBright"] = pumbbrightnessValue;
      doc["heaterSwitch"] = isHeaterOn?"ON":"OFF";;
      int tempPin = analogRead(TEMPERATURE_PIN);
      doc["temperature"] = tempPin;
      String output;
      serializeJson(doc, output);
      Serial.println(output);
      client.publish(DEVICE_STATUS, output.c_str());
    }
  } else {
    Serial.println("Other");
  }
}

void reconnect() {
  // Loop until we're reconnected
  int i = 0;
  digitalWrite(LIGHT_PIN, LOW);
  digitalWrite(PUMB_PIN, LOW);
  digitalWrite(HEATER_PIN, LOW);
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
      // Once connected, publish an announcement...
      const char* publishData = getPublishContent().c_str();
      client.publish(DISCOVER_CHANNEL, publishData);
      // ... and resubscribe  
      client.subscribe(LIGHT_COMMAND_CHANNEL);
      client.subscribe(LIGHT_BRIGHT_COMMAND_CHANNEL);
      client.subscribe(PUMB_COMMAND_CHANNEL);
      client.subscribe(PUMB_BRIGHT_COMMAND_CHANNEL);
      client.subscribe(HEATER_COMMAND_CHANNEL);
      client.subscribe(TARGET_COMMAND_SET_LOW_CHANNEL);
      client.subscribe(TARGET_COMMAND_SET_HIGH_CHANNEL);

      client.subscribe(DEVICE_COMMAND);
      
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
      writeFile("/network.txt", (strClientId + "," + strPwd + "," + mqtt_server).c_str());
      SPIFFS.end();

      digitalWrite(HEATER_PIN, isHeaterOn?HIGH:LOW);
      if (isOn) {
        analogWrite(LIGHT_PIN, brightnessValue);
      } else {
        digitalWrite(LIGHT_PIN, LOW);
      }
      
      analogWrite(PUMB_PIN, 255);
      delay(1000);
      analogWrite(PUMB_PIN, pumbbrightnessValue);
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
String getPublishContent() {
  String namePart = "{\"name\": \"";
  String uniqueIdPart = "\",\"unique_id\": \"";
  String cmd_TPart = "\",\"cmd_t\": \"";
  String stat_TPart = "\",\"stat_t\": \"";
  String otherPart = "\",\"pl_on\": \"on\",\"pl_off\": \"off\"}";
  String pubContent = namePart  + DEVICE_NAME + uniqueIdPart + DEVICE_NAME + cmd_TPart + LIGHT_COMMAND_CHANNEL + stat_TPart + LIGHT_STATE_CHANNEL + otherPart;
  return pubContent;
}
void heartBeat(PubSubClient *client) {
//  String publishStr = "{\"status\":\"";
//  publishStr.concat(client->state());
//  publishStr.concat("\", \"time\":");
//  publishStr.concat(millis());
//  publishStr.concat("}");
//  Serial.print("Publish heart beat: ");
//  Serial.print(publishStr);
//  Serial.println();
  // client->publish(STATE_CHANNEL, publishStr.c_str());
  // client->publish(STATE_CHANNEL, "online");
  Serial.print("Publish heart beat: ");
  Serial.print(STATE_CHANNEL);
  Serial.print(":");
  Serial.print("online");
  Serial.println();
//  if (isOn) {
//    client->publish(SEND_CHANNEL, "on");
//    client->publish(STATE_CHANNEL, "on");
//  } else {
//    client->publish(SEND_CHANNEL, "off");
//    client->publish(STATE_CHANNEL, "off");
//  }
  
}
void publishStatus(PubSubClient *client) {
  int tempPin = analogRead(TEMPERATURE_PIN);
  Serial.print("Publish temperature: ");
  Serial.print("Low:");
  Serial.println(tempLow);
  Serial.print("High:");
  Serial.println(tempHigh);
  Serial.print("CurrentPin:");
  Serial.println(tempPin);
  String tempString(tempPin);
  char tempValueStrChar[tempString.length()];
  tempString.toCharArray(tempValueStrChar, tempString.length() + 1);
  client->publish(TEMP_CHANNEL, tempValueStrChar);
//  
//  if (isHeaterOn) {
//    if (temperature > tempHigh) {
//      digitalWrite(HEATER_PIN, LOW);
//      isHeaterOn = false;
//    }
//  } else {
//    if (temperature < tempLow) {
//      digitalWrite(HEATER_PIN, HIGH);
//      isHeaterOn = true;
//    }
//  }
//  client->publish(HEATER_STATE_CHANNEL, isHeaterOn?"on":"off");
}
void resetNetworkButtonClicked()
{
  SPIFFS.begin();
  SPIFFS.remove("/network.txt");
  SPIFFS.end();
  Serial.println("Network reset Clicked");
  resetNetworkSettingsWhenFailed();
}
int data = 1;
void buttonClicked()
{
  Serial.println("Switch Button Clicked");
  client.publish(LIGHT_COMMAND_CHANNEL, "off");
//  resetNetworkSettingsWhenFailed();
} // doubleClick

void setup() {
  Serial.begin(115200);
  Serial.println("begin start littleFS");
  if (!SPIFFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }

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
    mqtt_server = MQTT_BROKER_HOST;
  }
  SPIFFS.end();
  Serial.println("end start littleFS");
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(LIGHT_PIN, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(PUMB_PIN, OUTPUT);
  pinMode(HEATER_PIN, OUTPUT);
  pinMode(TEMPERATURE_PIN, INPUT); 
  digitalWrite(LIGHT_PIN, LOW);
  digitalWrite(BUILTIN_LED, LOW);
  
  attachInterrupt(digitalPinToInterrupt(RESET_NETWORK_BUTTON_PIN), onNetworkButton, CHANGE);
//  attachInterrupt(digitalPinToInterrupt(SWITCH_BUTTON_PIN), onSwitchButton, CHANGE);

  resetNetworkButton.attachClick(resetNetworkButtonClicked);
  switchButton.attachClick(buttonClicked);
  
  setup_wifi();
  const char *clientIP = mqtt_server.c_str();
  Serial.println("connecting mqtt server");
  Serial.println(clientIP);
  Serial.println("mqtt port");
  Serial.println(MQTT_BROKER_PORT);
  client.setServer(clientIP, MQTT_BROKER_PORT);
  client.setCallback(callback);
  Serial.println("connected wifi");
}
ICACHE_RAM_ATTR void onNetworkButton() {
  Serial.println("onNetworkButton Clicked");
  resetNetworkButton.tick();
}
ICACHE_RAM_ATTR void onSwitchButton() {
  // include all buttons here to be checked
  Serial.println("onSwitchButton Clicked");
  switchButton.tick(); // just call tick() to check the state.
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  resetNetworkButton.tick();
  switchButton.tick();
  
  unsigned long now = millis();
  if ((now - lastHeartBeat) > 10000) {
    lastHeartBeat = now;
    heartBeat(&client);
    publishStatus(&client);
  }
  if ((now - lastStatusUpate) > 2000) {
    lastStatusUpate = now;

  }
}