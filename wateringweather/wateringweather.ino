#include "HAMqttWifiManager.h"
#include "HAMqttDeviceBase.h"
#include "HAMqttLight.h"
#include "HAMqttSwitch.h"
#include "HAMqttSensor.h"
#include "HAMqttServoGoAndBack.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <OneButton.h>
#include "AHT10_8266.h"
#include "BMP180_8266.h"
#include "HornerSensor_8266.h"
#include "Rain_8266.h"
#include "GY30_8266.h"

#define DEVICE_NAME "WatererAndWeather-e7170c3d"  // 院子右边浇水电磁阀


#define CLIENT_ID "WatererAndWeather-e7170c3d"
#define CLIENT_PASSWORD "waterandweather_123456"

#define WATERING_SWITCHER 14
#define WATERING_POWER_LIGHT 12
#define WEATHER_RAIN 16
#define WEATHER_HORNER 13


HAMqttWifiManager wm;
WiFiClient espClient;
PubSubClient* client = new PubSubClient(espClient);
HAMqttSwitch wateringSwither(client, WATERING_SWITCHER, "wateringswitcher", DEVICE_NAME, "homedevice/log");



long lastTime;
long currentTime;
long sensorGap = 3000;


void setup() {
  Serial.begin(115200);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  analogWriteFreq(40000);
  Serial.println("start");
  

  wateringSwither.setup();

  wm.setDeviceName(DEVICE_NAME);
  wm.setClientId(CLIENT_ID);

  wm.setClientPassword(CLIENT_PASSWORD);
  wm.setClientIP(String("192.168.50.109"));
  wm.setClientPort("9013");
  wm.setNotConnectedCallback(wmNotConnectCallBack);
  wm.setup();
  String mqttId = wm.getClientID();
  String mqttPassword = wm.getClientPassword();
  String mqttServerIP = wm.getClientIP();
  String mqttServerPort = wm.getClientPort();
  Serial.printf("mqttServerIP:%s port:%d", mqttServerIP.c_str(), mqttServerPort.toInt());
  client->setServer(mqttServerIP.c_str(), mqttServerPort.toInt());
  client->setCallback(callback);
  reconnectMqtt();
  wm.saveConfigToFile();
  lastTime = millis();
  digitalWrite(12, LOW);
}
void reconnectMqtt() {
  int i = 0;
  String mqttId = wm.getClientID();
  String mqttPassword = wm.getClientPassword();
  String mqttServerIP = wm.getClientIP();
  String mqttServerPort = wm.getClientPort();
  Serial.print("reconnecting with: ");
  Serial.print(mqttServerIP);
  Serial.print(":::::");
  Serial.print(mqttServerPort);
  Serial.print(":::::");
  Serial.print(mqttId);
  Serial.print(":::::");
  Serial.print(mqttPassword);
  while(!client->connected()) {
    client->setServer(mqttServerIP.c_str(), mqttServerPort.toInt());
    client->setCallback(callback);
    if (client->connect(mqttId.c_str(), "", mqttPassword.c_str())) {
      Serial.println("Connected to MQTT");
      break;
    }
    Serial.println("Connect to mqtt server failed, retrying");
    delay(12000);
    i++;
    if (i == 5) {
      break;
    }
  }
  if (i >= 5) {
    delay(5000);
    ESP.restart();
  }
  wateringSwither.doSubscribe();
  wateringSwither.publishDiscover();
  wateringSwither.publishSwitchStatus();
  wateringSwither.restoreStatus();
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
  wateringSwither.callback(topicString, payloadString);
}
void wmNotConnectCallBack() {
  Serial.println("loop-wm-noconnect-Set pin to 0 ");
  wateringSwither.beforeReconnect();
}
void loop() {
  if (!wm.isNetworkConnected() || !client->connected()) {
    Serial.println("loop-noconnect-network-Set pin to 0 ");
    wateringSwither.beforeReconnect();
    if (!wm.isNetworkConnected()) {
      wm.reconnect(-1);
    }
    reconnectMqtt();
  }
  client->loop();
  wateringSwither.loop();
  currentTime = millis();
  if (currentTime < lastTime) {
    lastTime = millis();
  } else if ((currentTime - lastTime) > sensorGap) {
    lastTime = millis();
  }
}
