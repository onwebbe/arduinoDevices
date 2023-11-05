#include "HAMqttWifiManager.h"
#include "HAMqttDeviceBase.h"
#include "HAMqttLight.h"
#include "HAMqttSwitch.h"
#include "HAMqttSensor.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define DEVICE_NAME "FlowerWater_test"
#define CLIENT_ID "FlowerWater_test"
#define CLIENT_PASSWORD "FlowerWater_123456"


#define PUMB_PIN D8
#define SWITCH_PIN LED_BUILTIN


HAMqttWifiManager wm;
WiFiClient espClient;
PubSubClient* client = new PubSubClient(espClient);
HAMqttLight light1(client, PUMB_PIN, "light", DEVICE_NAME, "homedevice/log");
HAMqttSwitch switch1(client, SWITCH_PIN, "pumb", DEVICE_NAME, "homedevice/log");
HAMqttSensor dht22(client, "device_sensor", DEVICE_NAME, "homedevice/log");
HAMqttSensorConfig* temperatureConfig;
HAMqttSensorConfig* humidityConfig;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("start");
  light1.setup();
  light1.setDelaySwichOff(90000);

  switch1.setup();
  switch1.setDelaySwichOff(60000);


  dht22.setup();
  temperatureConfig = new HAMqttSensorConfig("C", "temperature");
  humidityConfig = new HAMqttSensorConfig("%", "humidity");
  dht22.addSensorConfig(temperatureConfig);
  dht22.addSensorConfig(humidityConfig);


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
  int i = 0;
  while(!client->connected()) {
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
    wm.resetNetworkSettingsWhenFailed();
  }
  String channel = String("homedevice/#/") + String(DEVICE_NAME) + String("/+/status");
  // client->subscribe(channel.c_str());
  light1.doSubscribe();
  light1.publishDiscover();
  light1.publishSwitchStatus();
  light1.publishBrightStatus();

  switch1.doSubscribe();
  switch1.publishDiscover();
  switch1.publishSwitchStatus();

  dht22.doSubscribe();
  dht22.publishDiscover();
  dht22.publishData();
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
  switch1.callback(topicString, payloadString);
  dht22.callback(topicString, payloadString);
}

void loop() {
  if (!client->connected()) {
    Serial.println("loop-noconnect-Set pin to 0 ");
    light1.beforeReconnect();
    switch1.beforeReconnect();
    dht22.beforeReconnect();
    wm.reconnect();
    reconnectMqtt();
    light1.afterRconnected();
    switch1.afterRconnected();
    dht22.afterRconnected();
  }
  temperatureConfig->setData(String(random()));
  humidityConfig->setData(String(random()));
  client->loop();
  light1.loop();
  switch1.loop();
  dht22.loop();
}
