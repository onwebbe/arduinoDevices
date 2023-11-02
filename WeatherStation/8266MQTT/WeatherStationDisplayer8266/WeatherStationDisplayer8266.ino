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
#include <ArduinoJson.h>

#include "MQTT_8266.h"
#include "Utils_8266.h"

#define WIFI_NAME "WeatherStationDisplayerConfigAP"
#define DEFAULT_DEVICE_NAME "WeatherStationDisplayer"

MQTT_8266 mqtt = MQTT_8266("WeatherStationSensor", "sensor");
bool isAHT10 = false;
bool isGY30 = false;
bool isBMP180 = false;

WiFiManager wm;
bool shouldSaveConfig = false;

String MQTT_CLIENT_ID = "weatherStationDisplayer2";
String MQTT_CLIENT_PWD = "displayer123456";

OneButton btn = OneButton(
  2,  // Input pin for the button
  false,       // Button is active high
  false        // Disable internal pull-up resistor
);
static void handleClick() {
  Serial.println("Clicked!");
  Utils_8266::resetNetworkSettingsWhenFailed();
  
}
void setup() {
  Serial.begin(115200);
  Serial.println();
  btn.attachClick(handleClick);
  setup_wifi();
  //  String deviceName = wifiNetwork.getClientId();
  //  String devicePwd = wifiNetwork.getClientPassword();
    Utils_8266::setupWiFiManager(&wm);
//    mqtt.init("onwebbe.gicp.net", 9013, MQTT_CLIENT_ID, MQTT_CLIENT_PWD);
    mqtt.init("192.168.50.109", 9013, MQTT_CLIENT_ID, MQTT_CLIENT_PWD);
}

void loop() {
    btn.tick();
    mqtt.processInLoop();
    Serial.println("---------------------------------------------------------------");
    delay(1000);
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
//   wm.resetSettings();

  wm.setSaveConfigCallback(saveConfigCallback);
  wm.setConfigPortalTimeout(180);

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
  String wifiName = "WeatherStationDisplayerConfigAP";
  Serial.print("Creating wifi");
  Serial.print(wifiName);
  Serial.println();
  res = wm.autoConnect(wifiName.c_str(), ""); // password protected ap

  if(!res) {
      Serial.println("Failed to connect");
      Utils_8266::restartOnly();
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
