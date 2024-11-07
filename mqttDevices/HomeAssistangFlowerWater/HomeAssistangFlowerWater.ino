#include "HAMqttWifiManager.h"
#include "HAMqttDeviceBase.h"
#include "HAMqttLight.h"
#include "HAMqttSwitch.h"
#include "HAMqttSensor.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "OneButton.h"

#define DEVICE_NAME "FlowerWater"
#define CLIENT_ID "FlowerWater"
#define CLIENT_PASSWORD "FlowerWater_123456"


#define PUMB_PIN 14
#define SWITCH_PIN LED_BUILTIN


HAMqttWifiManager wm;
WiFiClient espClient;
PubSubClient* client = new PubSubClient(espClient);
HAMqttSwitch switch1(client, PUMB_PIN, "pumb", DEVICE_NAME, "homedevice/log");

OneButton networkResetButton(5, true);

ICACHE_RAM_ATTR void networkResetCheckTicks() {
  // include all buttons here to be checked
  networkResetButton.tick(); // just call tick() to check the state.
}
// this function will be called when the button was pressed 1 time only.
void singleClickNetworkResetButton() {
  Serial.println("networkResetSingleClicked() detected.");
  delay(2000);
  wm.resetNetworkSettingsWhenFailed();
} // singleClick

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("start");
  attachInterrupt(digitalPinToInterrupt(5), networkResetCheckTicks, CHANGE);
  networkResetButton.attachClick(singleClickNetworkResetButton);
  
  switch1.setup();
  switch1.setDelaySwichOff(20000);

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
    delay(5000);
    ESP.restart();
  }
  // String channel = String("homedevice/#/") + String(DEVICE_NAME) + String("/+/status");
  // client->subscribe(channel.c_str());

  switch1.doSubscribe();
  switch1.publishDiscover();
  switch1.publishSwitchStatus();
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
  switch1.callback(topicString, payloadString);
}
void wmNotConnectCallBack() {
  Serial.println("loop-wm-noconnect-Set pin to 0 ");
  switch1.beforeReconnect();
}
void loop() {
  if (!wm.isNetworkConnected() || !client->connected()) {
    Serial.println("loop-noconnect-Set pin to 0 ");
    switch1.beforeReconnect();
    if(!wm.isNetworkConnected()) {
      wm.reconnect(-1);
    }
    reconnectMqtt();
    switch1.afterRconnected();
  }
  client->loop();
  switch1.loop();
  networkResetButton.tick();
}
