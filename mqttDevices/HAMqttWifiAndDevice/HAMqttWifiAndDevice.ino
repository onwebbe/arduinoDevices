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

void setupSensorData(HAMqttSensor* sensor) {
  sensor->setSensorData("temperature", String(random()));
  sensor->setSensorData("humidity", String(random()));
}
void performActionAfterMQTTConnected(String wifiId) {
  light1.doSubscribe();
  light1.publishDiscover();
  light1.publishStatus();

  switch1.doSubscribe();
  switch1.publishDiscover();
  switch1.publishStatus();

  dht22.doSubscribe();
  dht22.publishDiscover();
  dht22.publishStatus();
}
void performActionBeforeDisConnected() {
  light1.beforeReconnect();
  switch1.beforeReconnect();
  dht22.beforeReconnect();
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("start");
  light1.setup();
  light1.setDelaySwichOff(90000);
  light1.setDiscoverTimeGap(30000);

  switch1.setup();
  switch1.setDelaySwichOff(60000);
  switch1.setDiscoverTimeGap(30000);


  dht22.setup();
  dht22.callToSetData(setupSensorData);
  dht22.setDiscoverTimeGap(30000);
  dht22.setPublishGap(10000);

  temperatureConfig = new HAMqttSensorConfig("C", "temperature");
  humidityConfig = new HAMqttSensorConfig("%", "humidity");
  dht22.addSensorConfig(temperatureConfig);
  dht22.addSensorConfig(humidityConfig);

  wm.setupMqttPubClient(client, callback);
  wm.setDeviceName(DEVICE_NAME);
  wm.setClientId(CLIENT_ID);

  wm.setClientPassword(CLIENT_PASSWORD);
  wm.setClientIP(String("192.168.50.109"));
  wm.setClientPort("9013");
  wm.setConnectSuccessfulCallback(performActionAfterMQTTConnected);
  wm.setBeforeNotConnectedCallback(performActionBeforeDisConnected);
  wm.setup();
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
  wm.loop();
  light1.loop();
  switch1.loop();
  dht22.loop();
}
