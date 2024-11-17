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

#define DEVICE_NAME "WatererAndWeather-333153da"  // 院子右边浇水电磁阀


#define CLIENT_ID "WatererAndWeather-333153da"
#define CLIENT_PASSWORD "waterandweather_123456"

#define WATERING_SWITCHER 14
#define WATERING_POWER_LIGHT 12
#define WEATHER_RAIN 16
#define WEATHER_HORNER 13


HAMqttWifiManager wm;
WiFiClient espClient;
PubSubClient* client = new PubSubClient(espClient);
HAMqttSwitch wateringSwither(client, WATERING_SWITCHER, "wateringswitcher", DEVICE_NAME, "homedevice/log");
HAMqttLight powerLight(client, WATERING_POWER_LIGHT, "power", DEVICE_NAME, "homedevice/log");

HAMqttSensor weatherData(client, "weatherData", DEVICE_NAME, "homedevice/log");

AHT10_8266 *ath10 = new AHT10_8266();

BMP180_8266 *bmp180 = new BMP180_8266();

HorerSensor_8266 *horner = new HorerSensor_8266(WEATHER_HORNER);

Rain_8266 *rain = new Rain_8266(WEATHER_RAIN);

GY30_8266 *gy30 = new GY30_8266();

HAMqttSensorConfig* temperatureConfig = new HAMqttSensorConfig("°C", "temperature");

HAMqttSensorConfig* humidityConfig = new HAMqttSensorConfig("%", "humidity");

HAMqttSensorConfig* presureConfig = new HAMqttSensorConfig("hPa", "presure");

HAMqttSensorConfig* lightConfig = new HAMqttSensorConfig("lx", "light");

HAMqttSensorConfig* windConfig = new HAMqttSensorConfig("mph", "wind");

HAMqttSensorConfig* rainConfig = new HAMqttSensorConfig("", "rain");


long sensorGap = 30000;

void populateSensorData(HAMqttSensor* sensor) {
  sensor->setSensorData("temperature", String(ath10->getTemperature()));
  sensor->setSensorData("humidity", String(ath10->getHumidity()));
  sensor->setSensorData("presure", String(bmp180->getPressure()));
  sensor->setSensorData("light", String(gy30->getGY30Data()));
  sensor->setSensorData("wind", String(horner->getAverageRPM()));
  sensor->setSensorData("rain", String(rain->isRain()));
}
void performActionAfterMQTTConnected(String wifiId) {
  wateringSwither.restoreStatus();
  weatherData.restoreStatus();
  powerLight.restoreStatus();
  
  wateringSwither.doSubscribe();
  wateringSwither.publishDiscover();
  wateringSwither.publishStatus();

  powerLight.doSubscribe();
  powerLight.publishDiscover();
  powerLight.publishStatus();

  weatherData.doSubscribe();
  weatherData.publishDiscover();
  weatherData.publishStatus();
}
void wmNotConnectCallBack() {
  Serial.println("loop-wm-noconnect-Set pin to 0 ");
  weatherData.beforeReconnect();
  wateringSwither.beforeReconnect();
  powerLight.beforeReconnect();
}
void setup() {
  Serial.begin(115200);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  analogWriteFreq(40000);
  Serial.println("start");
  ath10->setup();

  bmp180->setup();

  horner->setup();

  rain->setup();

  gy30->setup();

  temperatureConfig->setType(2);
  humidityConfig->setType(2);
  presureConfig->setType(2);
  presureConfig->setDevClass("pressure");
  lightConfig->setType(2);
  lightConfig->setDevClass("illuminance");
  windConfig->setType(2);
  windConfig->setDevClass("wind_speed");
  rainConfig->setType(2);
  rainConfig->setDevClass("water");
  weatherData.addSensorConfig(temperatureConfig);
  weatherData.addSensorConfig(humidityConfig);
  weatherData.addSensorConfig(presureConfig);
  weatherData.addSensorConfig(lightConfig);
  weatherData.addSensorConfig(windConfig);
  weatherData.addSensorConfig(rainConfig);

  wateringSwither.setup();
  wateringSwither.setDiscoverTimeGap(300000);
  wateringSwither.setDelaySwichOff(60000);
  powerLight.setup();
  powerLight.setDiscoverTimeGap(300000);
  weatherData.setup();
  weatherData.setDiscoverTimeGap(300000);
  weatherData.callToSetData(populateSensorData);
  weatherData.setPublishGap(sensorGap);

  wm.setupMqttPubClient(client, callback);
  wm.setDeviceName(DEVICE_NAME);
  wm.setClientId(CLIENT_ID);

  wm.setClientPassword(CLIENT_PASSWORD);
  wm.setClientIP(String("192.168.50.109"));
  wm.setClientPort("9013");
  wm.setNotConnectedCallback(wmNotConnectCallBack);
  wm.setConnectSuccessfulCallback(performActionAfterMQTTConnected);
  wm.setBeforeNotConnectedCallback(wmNotConnectCallBack);
  wm.setup();
  digitalWrite(12, LOW);
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
  powerLight.callback(topicString, payloadString);
  weatherData.callback(topicString, payloadString);
}

void loop() {
  wm.loop();
  wateringSwither.loop();
  powerLight.loop();
  weatherData.loop();
}
