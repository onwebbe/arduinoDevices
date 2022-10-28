/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Create a WiFi access point and provide a web server on it. */
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "DataDefinitions.h"
#include <FS.h>

#ifndef APSSID
//#define APSSID "Driver"
//#define APPSK  "1234554321"

#define WEBSOCKET_CLIENT_ROLE_CONTROLLER 1
#define WEBSOCKET_CLIENT_ROLE_RADAR 2
#endif

/* Set these to your desired credentials. */
//const char *ssid = APSSID;
//const char *password = APPSK;
const char* ssid = "Driver";
const char* password = "1234554321";
String js1FileName = "/2.34c64731.chunk.js";
String js2FileName = "/main.7f396308.chunk.js";
String js3FileName = "/runtime-main.3c729693.js";
String cssFileName = "/main.95dcb6cd.chunk.css";
// nodemcu using 1, 2, 3, 12, 13, 14, 15
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

ControllerData controllerData;
SensorData sensorData;
unsigned long currentTime = millis();
unsigned long heartBeatTime = millis();

void initData() {
  controllerData.leftController_UP_DOWN = 0;
  controllerData.leftController_LEFT_RIGHT = 0;
  controllerData.rightController_UP_DOWN = 0;
  controllerData.rightController_LEFT_RIGHT = 0;
  controllerData.switchA = 0;
  controllerData.switchB = 0;

  sensorData.rotateX = 0;
  sensorData.rotateY = 0;
  sensorData.rotateZ = 0;
  sensorData.distance = 0;
}

void getAllData() {
  String controllerJSON = controllerData.convertToJSON();
  String sensorJSON = sensorData.convertToJSON();
  
  StaticJsonDocument<200> docController;
  deserializeJson(docController, controllerJSON);
  
  StaticJsonDocument<2048> docSensor;
  deserializeJson(docSensor, sensorJSON);
  docSensor["controllerData"] = docController["controllerData"];

  String resultString = "";
  serializeJsonPretty(docSensor, resultString);
  server.send(200, "application/json", resultString);
}

/* Just a little test message.  Go to http://192.168.5.1 in a web browser
   connected to this access point to see it.
*/
void handleNotFound() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void postControllerData() {
  String data = server.arg(0);
  controllerData.getFromJSON(data);
  server.send(200, "text/html", "<h1>updated</h1>");
}

void getHTML() {
  File file=SPIFFS.open("/index.html","r");//以只读模式打开index.htm，流模式为text/html。然后关闭该文件
  server.streamFile(file,"text/html");
  file.close();
  // server.send(200, "text/html", getHTMLText());
}
void getCss() {
  File file=SPIFFS.open(cssFileName,"r");//以只读模式打开index.htm，流模式为text/html。然后关闭该文件
  server.streamFile(file,"text/css");
  file.close();
  // server.send(200, "text/css", getCssText());
}
void getJS1() {
  File file=SPIFFS.open(js1FileName,"r");//以只读模式打开index.htm，流模式为text/html。然后关闭该文件
  server.streamFile(file,"application/javascript");
  file.close();
  // server.send(200, "application/javascript", getJS1Text());
}
void getJS2() {
  File file=SPIFFS.open(js2FileName,"r");//以只读模式打开index.htm，流模式为text/html。然后关闭该文件
  server.streamFile(file,"application/javascript");
  file.close();
  // server.send(200, "application/javascript", getJS2Text());
}
void getJS3() {
  File file=SPIFFS.open(js3FileName,"r");//以只读模式打开index.htm，流模式为text/html。然后关闭该文件
  server.streamFile(file,"application/javascript");
  file.close();
  // server.send(200, "application/javascript", getJS3Text());
}
void getFavourIcon() {
  File file=SPIFFS.open("/favicon.ico","r");//以只读模式打开index.htm，流模式为text/html。然后关闭该文件
  server.streamFile(file,"application/x-ico");
  file.close();
  // server.send(200, "application/javascript", getJS3Text());
}
void setup() {
  Serial.begin(115200);
  Serial.println("-");
  delay(1000);
  Serial.println("-");
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  IPAddress local_IP(192,168,5,1);//手动设置的开启的网络的ip地址
  IPAddress gateway(192,168,5,1);  //手动设置的网关IP地址
  IPAddress subnet(255,255,255,0); //手动设置的子网掩码
  //设置为接入点模式
  WiFi.mode(WIFI_AP);

 //配置接入点的IP，网关IP，子网掩码
 WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.println("AP IP address");
  Serial.println(myIP);

  SPIFFS.begin();
  Serial.println("SPIFFS started");

  String cssF = "/static/css";
  cssF.concat(cssFileName);
  String jsF1 = "/static/js";
  jsF1.concat(js1FileName);
  String jsF2 = "/static/js";
  jsF2.concat(js2FileName);
  String jsF3 = "/static/js";
  jsF3.concat(js3FileName);
  
  server.on(cssF, getCss);
  server.on(jsF1, getJS1);
  server.on(jsF2, getJS2);
  server.on(jsF3, getJS3);
  
  server.on("/index.html", getHTML);
  server.on("/", getHTML);
  server.on("/favicon.ico", getFavourIcon);
  server.onNotFound(handleNotFound);

  webSocket.onEvent(webSocketEvent);
  
  server.begin();
  webSocket.begin();
  
  Serial.println("HTTP server started at 80");
  Serial.println("Websocket server started at 81");
  initData();
  
  
//
  servo1.attach(0);
  servo2.attach(2);
  servo3.attach(13);
  servo4.attach(12);
  servo5.attach(3);
  servo6.attach(14);
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);
  servo5.write(90);
  servo6.write(90);
  Serial.println("Initial Servo Done");
}
// int clientNum = 0;
void loop() {
  server.handleClient();
  webSocket.loop();
  if ((millis() - currentTime) >= 10) {
    currentTime = millis();
    if (currentTime >= 0) {
      // webSocket.sendTXT(clientNum, test2);
    }
  }
  if (millis() - heartBeatTime > 500) {
     servo1.write(0);
     // Serial.println("!!!!!!!!!!!!!!!!!!!! Lost Connection !!!!!!!!!!!!!!!!!!!!");
  }
}

/*
 * {
 *   "command": "setController",
 *   "clientType": "controller",
 *   "data": {}
 * }
 * 
 * {
 *   "command": "getSensorData",
 *   "clientType": "sensor",
 *   "data": {}
 * }
 */
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            servo1.write(0);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            }
            break;
        case WStype_TEXT:
            {
                // Serial.printf("[%u] get Text: %s\n", num, payload);
                Serial.printf("%s\n", payload);
                StaticJsonDocument<500> doc;
                // DynamicJsonDocument doc(2048);
                DeserializationError error = deserializeJson(doc, payload);
                if (error) {
                  Serial.print(F("deserializeJson() failed: "));
                  Serial.println(error.f_str());
                  return;
                }
                JsonObject jsonObj = doc.as<JsonObject>();
                String command = jsonObj["command"];
                if (command.compareTo("setController") == 0) {
                  if (millis() - heartBeatTime > 500) {
                     servo1.write(0);
                     // Serial.println("!!!!!!!!!!!!!!!!!!!! Lost Connection !!!!!!!!!!!!!!!!!!!!");
                  } else {
                    setControllerData(jsonObj["data"]);
                  }
                  heartBeatTime = millis();
                } else if (command.compareTo("setConfig") == 0) {
                  setConfigData(jsonObj["data"]);
                }
                // send message to client
                // webSocket.sendTXT(num, "message here");
    
                // send data to all connected clients
                // webSocket.broadcastTXT("message here");
            }
            break;
        case WStype_BIN:
            Serial.printf("[%u] get binary length: %u\n", num, length);
            hexdump(payload, length);

            // send message to client
            // webSocket.sendBIN(num, payload, length);
            break;
    }
}
void setConfigData(JsonObject data) {
  Serial.println("setConfigControllerData");
}
void setControllerData(JsonObject data) {
//  Serial.println("setControllerData");
  
  controllerData.getFromJSONObj(data);
  
// Serial.printf("UD: %d LR: %d strength: %d left: %d right: %d", rightUD, rightLR, rightStrength, leftValue, rightValue);
  int leftUD = map(controllerData.leftController_UP_DOWN, 0, 250, 0, 180);
  int leftLR = map(controllerData.leftController_LEFT_RIGHT, 0, 250, 0, 180);
  int rightUD = map(controllerData.rightController_UP_DOWN, 0, 250, 0, 180);
  int rightLR = map(controllerData.rightController_LEFT_RIGHT, 0, 250, 0, 180);
  int sw1 = map(controllerData.switchA, 0, 100, 0, 180);
  int sw2 = map(controllerData.switchB, 0, 100, 0, 180);
  servo1.write(leftUD);
  servo2.write(leftLR);
  servo3.write(rightUD);
  servo4.write(rightLR);
  servo5.write(sw1);
  servo6.write(sw2);
}
