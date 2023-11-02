#include "MyMqttWifiManager.h"
#include "MyMqttLight.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define DEVICE_NAME "FlowerWater_1"
#define CLIENT_ID "FlowerWater_1"
#define CLIENT_PASSWORD "FlowerWater_123456"


#define PUMB_PIN D8


MqttWifiManager wm;
String mqttId;
String mqttPassword;
String mqttServerIP;
WiFiClient espClient;
PubSubClient* client = new PubSubClient(espClient);
MqttLight light1(client, PUMB_PIN, DEVICE_NAME, "Garden-Flower", "homedevice/log");

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("start");
  light1.setup();
  light1.setDelaySwichOff(90000);
  wm.setDeviceName(DEVICE_NAME);
  wm.setClientId(CLIENT_ID);

  wm.setClientPassword(CLIENT_PASSWORD);
  wm.setClientIP(String("192.168.50.109"));
  wm.setClientPort("9013");
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
}
void reconnectMqtt() {
  String mqttId = wm.getClientID();
  String mqttPassword = wm.getClientPassword();
  String mqttServerIP = wm.getClientIP();
  int i = 0;
  while(!client->connected()) {
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
    wm.resetNetworkSettingsWhenFailed();
  }
  String channel = String("homedevice/#/") + String(DEVICE_NAME) + String("/+/status");
  // client->subscribe(channel.c_str());
  light1.doSubscribe();
  light1.publishDiscover();
  light1.publishSwitchStatus();
  light1.publishBrightStatus();
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
  light1.callback(topicString, payloadString);
}

void loop() {
  if (!client->connected()) {
    Serial.println("loop-noconnect-Set pin to 0 ");
    light1.beforeReconnect();
    wm.reconnect();
    reconnectMqtt();
    light1.afterRconnected();
  }
  client->loop();
  light1.loop();
}
