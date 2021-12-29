#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <WebSocketsServer.h>
#include "Radar8266.h"

#define LEFT_A 0
#define LEFT_B 2
#define LEFT_SPEED 13
#define RIGHT_A 5
#define RIGHT_B 4
#define RIGHT_SPEED 3

//#define PORT1 25;
//#define PORT2 26;
//#define PORT3 32;
//#define PORT4 33;

const int led = 13;


const char* ssid = "pankey_asus";
const char* password = "1234554321";
Radar8266 radar(14, 12, 16);
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
int clientNum;
void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(LEFT_A, OUTPUT);
  pinMode(LEFT_B, OUTPUT);
  pinMode(RIGHT_A, OUTPUT);
  pinMode(RIGHT_B, OUTPUT);
  pinMode(LEFT_SPEED, OUTPUT);
  pinMode(RIGHT_SPEED, OUTPUT);
  delay(1000);

  Serial.begin(115200);
//  WiFi.softAP(ssid, password);
//  IPAddress myIP = WiFi.softAPIP();
//  Serial.println(myIP);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


//
//  server.on("/inline", []() {
//    server.send(200, "text/plain", "this works as well");
//  });

  server.onNotFound(handleNotFound);

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("HTTP server started");
}
unsigned long lastTime = millis();
int angle = 0;
int isGo = 0;

void loop(void) {
  server.handleClient();
  webSocket.loop();

  if ((millis() - lastTime) > 20) {
    if (isGo == 0) {
      angle = angle + 10;
    } else {
      angle = angle - 10;
    }
    if (angle == 180) {
      isGo = 1;
    }
    if (angle == 0) {
      isGo = 0;
    }
    radar.rotateAndScan(angle);
    // webSocket.sendTXT(clientNum, "");
    lastTime = millis();
  }
}
String getRadarData() {
  float* result = radar.getRandarDataGo();
  String jsonResult;

  StaticJsonDocument<1024> rootDoc;
  for(int i = 0; i <= 18; i++) {
    JsonObject object = rootDoc.createNestedObject();
    object["index"] = i * 10;
    float distance = result[i];
    object["distance"] = distance;
  }
  serializeJson(rootDoc, jsonResult);
  return jsonResult;
}
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            clientNum = num;
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        
        // send message to client
        webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
            {
              Serial.printf("[%u] get Text: %s\n", num, payload);
              StaticJsonDocument<200> doc;
              DeserializationError error = deserializeJson(doc, payload);
              if (error.code() == DeserializationError::Ok) {
                int x = doc["x"];
                int y = doc["y"];
                int distance = doc["distance"];
                int mapDistance = map(distance, 0, 110, 0, 255);
                double leftSpeed = mapDistance;
                double rightSpeed = mapDistance;
                double ratio = 1 - ((double)abs(x) / (double)110);
                if (ratio > 0.8) { ratio = 0; }
                ratio = 1 - ratio;
                Serial.println(ratio);
                if(x > 0) {
                  if (y > 0) {
                    digitalWrite(LEFT_A, LOW);
                    digitalWrite(LEFT_B, HIGH);
                    digitalWrite(RIGHT_A, LOW);
                    digitalWrite(RIGHT_B, HIGH);
                  } else {
                    digitalWrite(LEFT_A, HIGH);
                    digitalWrite(LEFT_B, LOW);
                    digitalWrite(RIGHT_A, HIGH);
                    digitalWrite(RIGHT_B, LOW);
                  }
                  leftSpeed = mapDistance * ratio;
                  if (ratio == 1) {
                    rightSpeed = leftSpeed;
                  } else {
                    rightSpeed = leftSpeed * ( 1 - ratio );
                  }
                } else {
                  if (y > 0) {
                    digitalWrite(LEFT_A, LOW);
                    digitalWrite(LEFT_B, HIGH);
                    digitalWrite(RIGHT_A, LOW);
                    digitalWrite(RIGHT_B, HIGH);
                  } else {
                    digitalWrite(LEFT_A, HIGH);
                    digitalWrite(LEFT_B, LOW);
                    digitalWrite(RIGHT_A, HIGH);
                    digitalWrite(RIGHT_B, LOW);
                  }
                  rightSpeed = mapDistance * ratio;
                  if (ratio == 1) {
                    leftSpeed = rightSpeed;
                  } else {
                    leftSpeed = rightSpeed * ( 1 - ratio );
                  }
                }
                //PWM 0 - 1024
                Serial.println(leftSpeed);
                Serial.println(rightSpeed);
                analogWrite(LEFT_SPEED, rightSpeed);
                analogWrite(RIGHT_SPEED, leftSpeed);
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
