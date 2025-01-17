

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

#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <OneButton.h>
#include <FS.h>

#define DEVICE_NAME "light_yugang1"
#define STATE_CHANNEL "homedevice/yugang1/light/status"
#define SEND_CHANNEL "homedevice/yugang1/light/set"
#define RECEIVE_CHANNEL "homedevice/yugang1/light/set"
#define DISCOVER_CHANNEL "homedevice/yugang1/light/config"

#define TARGET_COMMAND_CHANNEL "homedevice/yugang1/light/set"

//#define CLIENT_ID "ESP8266Client-522b"
//#define CLIENT_PASSWORD "autogenerated_token_522b_7fb50d48-342a-11ed-a261-0242ac120002"
#define MQTT_BROKER_HOST "192.168.50.109"
#define MQTT_BROKER_PORT 9013

// #define WIFI_NAME "HUAWEI—7539"
#define WIFI_NAME "pankey_asus"
#define WIFI_PASSWORD "1234554321"
#define LIGHT_PIN 16
#define RESET_NETWORK_BUTTON_PIN 12
#define SWITCH_BUTTON_PIN 16

#define CLIENT_ID "HomeAssistantLight_yugang_1"
#define  CLIENT_PASSWORD "yugang_1"
// Update these with values suitable for your network.

const char* ssid = WIFI_NAME;
const char* password = WIFI_PASSWORD;
const char* mqtt_server = MQTT_BROKER_HOST;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
boolean shouldSaveConfig = false;

WiFiManager wm;
OneButton resetNetworkButton(RESET_NETWORK_BUTTON_PIN, true);
OneButton switchButton(SWITCH_BUTTON_PIN, true);
boolean isOn = false;
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
  WiFiManagerParameter custom_client_id("clientid", "Client Id", clientid, 50);
  WiFiManagerParameter custom_client_pwd("clientpwd", "Client Pass", CLIENT_PASSWORD, 100);
  wm.addParameter(&custom_client_id);
  wm.addParameter(&custom_client_pwd);
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  String strDeviceName = DEVICE_NAME;
  String wifiName = "8266Light-" + strDeviceName;
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
  Serial.println("--------get wifi ssid--------");
  Serial.println(wm.getWiFiSSID());
  String client_id = custom_client_id.getValue();
  String client_password = custom_client_pwd.getValue();
  Serial.print("CLIENT_ID: ");
  Serial.print(client_id);
  Serial.print(":::::");
  Serial.println();
  Serial.print("CLIENT_PASSWORD: ");
  Serial.print(client_password);
  Serial.print(":::::");
  Serial.println();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.print("[");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.print("]");
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if (payload[1] == 'f') {
    
    Serial.println("Switch OFF");
    digitalWrite(LIGHT_PIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    digitalWrite(BUILTIN_LED, HIGH);
    // client.publish(TARGET_COMMAND_CHANNEL, "on");
    client.publish(STATE_CHANNEL, "off");
    isOn = false;
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    Serial.println("Switch ON");
    digitalWrite(LIGHT_PIN, HIGH);  // Turn the LED off by making the voltage HIGH
    digitalWrite(BUILTIN_LED, LOW);
    // client.publish(TARGET_COMMAND_CHANNEL, "off");
    client.publish(STATE_CHANNEL, "on");
    isOn = true;
  }
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
      // Once connected, publish an announcement...
      const char* publishData = getPublishContent().c_str();
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
  String pubContent = namePart  + DEVICE_NAME + uniqueIdPart + DEVICE_NAME + cmd_TPart + RECEIVE_CHANNEL + stat_TPart + STATE_CHANNEL + otherPart;
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
  client.publish(TARGET_COMMAND_CHANNEL, "off");
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
    Serial.print("get setting file");
    Serial.print(client_id);
    Serial.println();
    Serial.print(client_password);
    Serial.println();
  }
  SPIFFS.end();
  Serial.println("end start littleFS");
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(LIGHT_PIN, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(LIGHT_PIN, LOW);
  digitalWrite(BUILTIN_LED, LOW);
  
  attachInterrupt(digitalPinToInterrupt(RESET_NETWORK_BUTTON_PIN), onNetworkButton, CHANGE);
  attachInterrupt(digitalPinToInterrupt(SWITCH_BUTTON_PIN), onSwitchButton, CHANGE);

  resetNetworkButton.attachClick(resetNetworkButtonClicked);
  switchButton.attachClick(buttonClicked);
  
  setup_wifi();
  client.setServer(mqtt_server, MQTT_BROKER_PORT);
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
  if (now - lastMsg > 10000) {
    lastMsg = now;
    heartBeat(&client);
  }
}
