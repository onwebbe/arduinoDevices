#include <ArduinoJson.h>
#include "WeatherDataRetriver.h"
#include <WeatherDisplay.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#define DHT22PIN 16
#define HORNERPIN 17
#define RAINPIN 18
const char* ssid = "pankey_asus";
const char* password = "1234554321";
IPAddress localIPAddress;
WebServer server(4480);
WeatherDataRetriver retriver = WeatherDataRetriver(DHT22PIN, HORNERPIN, RAINPIN);
void setup() {
  Serial.begin(9600);
  Serial.println("start");
  retriver.init();
  Serial.println("start device done");
  Serial.println("start wifi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  localIPAddress = WiFi.localIP();

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  server.on("/weatherData", handleWeatherData);
  server.begin();
  Serial.println("start wifi done");
}
void handleWeatherData() {
  Serial.println("start web get data");
  String data = retriver.getWeatherData();
  server.send(200, "text/json", data);
  Serial.println("end web get data");  
}
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("reconnecting wifi");
    ESP.restart();
//    WiFi.mode(WIFI_STA);
//    WiFi.begin(ssid, password);
//    while (WiFi.status() != WL_CONNECTED) {
//      delay(500);
//      Serial.print(".");
//    }
//    Serial.print("Connected to ");
//    Serial.println(ssid);
//    Serial.print("IP address: ");
//    Serial.println(WiFi.localIP());
//    localIPAddress = WiFi.localIP();
//    Serial.println("restart wifi done");
  }
  server.handleClient();

  // no need to display at this module so remove below
//  Serial.println("start loop");
//  Serial.println(localIPAddress);
//  Serial.println("start get data");
//  String data = retriver.getWeatherData();
//  Serial.println("end loop");
//  Serial.println();
//  Serial.println(data);
//  Serial.println();

  
//  StaticJsonDocument<200> doc;
//  deserializeJson(doc, data);
//  const char* temperature = doc["temperature"];
//  const char* humidity = doc["humidity"];
//  const char* pressure = doc["pressure"];
//  const char* light = doc["light"];
//  const char* wind = doc["wind"];
//  Serial.println("end loop");
//  delay(1000);
  
}
