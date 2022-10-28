#include "Utils_8266.h"
#include "AHT10_8266.h"
#include "GY30_8266.h"
#include "HorerSensor_8266.h"
#include "Rain_8266.h"
#include <Arduino.h>
#include <Wire.h>
#include "BMP180_8266.h"

#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <OneButton.h>
#include <FS.h>
#include <ESP8266WiFi.h>

#include "MQTT_8266.h"


#define WIFI_NAME "WeatherStationConfigAP"
#define DEFAULT_DEVICE_NAME "WeatherStationSensor"

HorerSensor_8266 horerSensor = HorerSensor_8266(13);
Rain_8266 rain = Rain_8266(16);
MQTT_8266 mqtt = MQTT_8266("WeatherStationSensor", "sensor");
bool isAHT10 = false;
bool isGY30 = false;
bool isBMP180 = false;

WiFiManager wm;
bool shouldSaveConfig = false;

String MQTT_CLIENT_ID;
String MQTT_CLIENT_PWD;

void setup() {
  Serial.begin(115200);
  Serial.println();

  Wire.begin();
  if (! AHT10_8266::begin()) {
    Serial.println(F("AHT10 not detected!"));
  } else {
    isAHT10 = true;
  }
  if (! GY30_8266::begin()) {
    Serial.println(F("GY30 not detected!"));
  } else {
    isGY30 = true;
  }
  if (!BMP180_8266::begin()) {
    Serial.println(F("BMP180 not detected!"));
  } else {
    isBMP180 = true;
  }
  horerSensor.init();
  
  setup_wifi();
  //  String deviceName = wifiNetwork.getClientId();
  //  String devicePwd = wifiNetwork.getClientPassword();
    Utils_8266::setupWiFiManager(&wm);
    mqtt.init("onwebbe.gicp.net", 9013, MQTT_CLIENT_ID, MQTT_CLIENT_PWD);
//    mqtt.init("192.168.50.109", 9013, MQTT_CLIENT_ID, MQTT_CLIENT_PWD);
}
void checkWifi() {
  WiFiClient client;
  if (!client.connect("www.baidu.com", 80)) {
      Serial.println("connection failed");
      Utils_8266::restartOnly();
      return;
  }
}
void loop() {
    checkWifi();
    Serial.print("isRain:");
    Serial.println(rain.isRain());
    Serial.println();
    if(rain.isRain()) {
      mqtt.publish_binary_sensor_data("ON");
    } else {
      mqtt.publish_binary_sensor_data("OFF");
    }
    float temperature = 0;
    float humidity = 0;
    float pressure = 0;
    float wind_speed = 0;
    int light = 0;
    if (isAHT10) {
      temperature = AHT10_8266::getTemperature();
      humidity = AHT10_8266::getHumidity();
      Serial.print(temperature);
      Serial.println(" C");
      Serial.println();
  
      Serial.print(humidity);
      Serial.println(" %");
      Serial.println();
    }
  
    if (isGY30) {
      light = GY30_8266::getGY30Data();
      Serial.print(light);
      Serial.println(" LUX");
      Serial.println();
    }
  
    if (isBMP180) {
      pressure = BMP180_8266::getPressure();
      Serial.print(pressure);
      Serial.println(" hPa");
      Serial.println();
    }
    wind_speed = horerSensor.getAverageRPM();
    
    String data = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + ",\"pressure\":" + String(pressure) + ",\"wind_speed\":" + String(wind_speed) + ",\"light\":" + String(light) + "}";
    mqtt.publish_data(data);
    mqtt.processInLoop();
    Serial.println("---------------------------------------------------------------");
    delay(3000);
}

void setup_wifi() {
  
  Serial.println("begin start littleFS");
  if (!SPIFFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }
  if (SPIFFS.exists("/network.txt")) {
    Serial.println("network setting file exists");
    String strRaw = Utils_8266::readFile("/network.txt");
    String splited[2];
    Utils_8266::Split(strRaw, ",", splited);
    MQTT_CLIENT_ID = splited[0];
    MQTT_CLIENT_PWD = splited[1];
    Serial.print("get setting file");
    Serial.print(MQTT_CLIENT_ID);
    Serial.println();
    Serial.print(MQTT_CLIENT_PWD);
    Serial.println();
  }
  SPIFFS.end();
  Serial.println("end start littleFS");
  
  Serial.println("done network init()");

  delay(10);
  // We start by connecting to a WiFi network
  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
//  WiFiManager wm;
  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
//  wm.resetSettings();

  wm.setSaveConfigCallback(saveConfigCallback);

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result
  char clientid[] = "";
  char clientpwd[] = "";
  
  WiFiManagerParameter custom_client_id("clientid", "Client Id", MQTT_CLIENT_ID.c_str(), 50);
  WiFiManagerParameter custom_client_pwd("clientpwd", "Client Pass", MQTT_CLIENT_PWD.c_str(), 100);
  wm.addParameter(&custom_client_id);
  wm.addParameter(&custom_client_pwd);
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  String strDeviceName = "test";
  String wifiName = "WeatherStationConfigAP";
  Serial.print("Creating wifi");
  Serial.print(wifiName);
  Serial.println();
  res = wm.autoConnect(wifiName.c_str(), ""); // password protected ap

  if(!res) {
      Serial.println("Failed to connect");
      Utils_8266::resetNetworkSettingsWhenFailed();
  }
  MQTT_CLIENT_ID = custom_client_id.getValue();
  MQTT_CLIENT_PWD = custom_client_pwd.getValue();
  Serial.println("connected...yeey :)");
  Serial.println(custom_client_id.getValue());
  Serial.println(custom_client_pwd.getValue());
  Serial.println("--------get wifi ssid--------");
  Serial.println(wm.getWiFiSSID());
//  CLIENT_ID = custom_client_id.getValue();
//  CLIENT_PASSWORD = custom_client_pwd.getValue();
//  Serial.print("CLIENT_ID: ");
//  Serial.print(CLIENT_ID);
//  Serial.print(":::::");
//  Serial.println();
//  Serial.print("CLIENT_PASSWORD: ");
//  Serial.print(CLIENT_PASSWORD);
//  Serial.print(":::::");
//  Serial.println();
}
void saveConfigCallback() {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}
