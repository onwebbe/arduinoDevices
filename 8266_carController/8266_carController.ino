#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "Radar8266.h"
#include <ArduinoJson.h>

#define LEFT_A 0
#define LEFT_B 2
#define LEFT_SPEED 13
#define RIGHT_A 5
#define RIGHT_B 4
#define RIGHT_SPEED 3

const char* ssid = "pankey_asus";
const char* password = "1234554321";

Radar8266 radar(14, 12, 16);

ESP8266WebServer server(80);  


const int led = 13;


int currentSpeed = 500;
int maxSpeed = 1024;
int minSpeed = 0;

void turnLeft() {
  digitalWrite(LEFT_A, LOW);
  digitalWrite(LEFT_B, LOW);
  digitalWrite(RIGHT_A, HIGH);
  digitalWrite(RIGHT_B, LOW);
  server.send(200, "text/plain", "MoveLeft");
}
void turnRight() {
  digitalWrite(LEFT_A, HIGH);
  digitalWrite(LEFT_B, LOW);
  digitalWrite(RIGHT_A, LOW);
  digitalWrite(RIGHT_B, LOW);
  server.send(200, "text/plain", "MoveRight");
}
void moveForward() {
  digitalWrite(LEFT_A, HIGH);
  digitalWrite(LEFT_B, LOW);
  digitalWrite(RIGHT_A, HIGH);
  digitalWrite(RIGHT_B, LOW);
  server.send(RIGHT_A, "text/plain", "MoveForward");
}
void moveBackward() {
  digitalWrite(LEFT_A, LOW);
  digitalWrite(LEFT_B, HIGH);
  digitalWrite(RIGHT_A, LOW);
  digitalWrite(RIGHT_B, HIGH);
  server.send(200, "text/plain", "MoveBackward");
}
void noMove() {
  digitalWrite(LEFT_A, LOW);
  digitalWrite(LEFT_B, LOW);
  digitalWrite(RIGHT_A, LOW);
  digitalWrite(RIGHT_B, LOW);
  server.send(200, "text/plain", "NoMove");
}
void speedUp() {
  if ((currentSpeed + 100) <= maxSpeed) {
    currentSpeed = currentSpeed + 100;
    analogWrite(LEFT_SPEED, currentSpeed);
    analogWrite(RIGHT_SPEED, currentSpeed);
  }

//  char *finalSpeedJSONStr = "{\"speedUp\": ";
//  char *speedChar;
//  itoa(currentSpeed, speedChar, 10);
//  strcat(finalSpeedJSONStr, speedChar);
//  strcat(finalSpeedJSONStr, "}");
  server.send(200, "application/json", "SpeedUp");
}
void speedDown() {
  if ((currentSpeed - 100) >= minSpeed) {
    currentSpeed = currentSpeed - 100;
    analogWrite(LEFT_SPEED, currentSpeed);
    analogWrite(RIGHT_SPEED, currentSpeed);
  }
//  char *finalSpeedJSONStr = "{\"speedDown\": ";
//  char *speedChar;
//  itoa(currentSpeed, speedChar, 10);
//  strcat(finalSpeedJSONStr, speedChar);
//  strcat(finalSpeedJSONStr, "}");
  server.send(200, "application/json", "speedDown");
}
char* dtostr(char *str, double d)
{
    sprintf(str, "%f", d);
    return str;
}
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
  server.send(400, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  Serial.begin(9600);
  Serial.println("start");
  
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

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/api/car/direction/turnLeft", turnLeft);
  server.on("/api/car/direction/turnRight", turnRight);
  server.on("/api/car/direction/moveForward", moveForward);
  server.on("/api/car/direction/moveBackward", moveBackward);
  server.on("/api/car/direction/noMove", noMove);
  server.on("/api/car/direction/speedUp", speedUp);
  server.on("/api/car/direction/speedDown", speedDown);

  server.on("/html/radar", []() {
    server.send(200, "text/html", getHTML());
  });

  server.on("/api/car/radarData", []() {
    String radarJSONString = getRadarData();
    server.send(200, "text/json", radarJSONString);
  });

//  server.on("/inline", []() {
//    server.send(20RIGHT_A, "text/plain", "this works as well");
//  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  pinMode(LEFT_A, OUTPUT);
  pinMode(LEFT_B, OUTPUT);
  pinMode(RIGHT_A, OUTPUT);
  pinMode(RIGHT_B, OUTPUT);
  pinMode(LEFT_SPEED, OUTPUT);
  pinMode(RIGHT_SPEED, OUTPUT);
  analogWriteRange(1024);
  analogWrite(LEFT_SPEED, currentSpeed);
  analogWrite(RIGHT_SPEED, currentSpeed);
//  pinMode(LEFT_A, OUTPUT);
//  pinMode(6, OUTPUT);
//  pinMode(7, OUTPUT);
//  pinMode(8, OUTPUT);
//  delay(1000);
  
}
unsigned long lastTime = millis();
int angle = 0;
int isGo = 0;
void loop(void) {
  server.handleClient();
  if ((millis() - lastTime) > 50) {
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
    lastTime = millis();
  }
  delay(2);//allow the cpu to switch to other tasks
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
String getRadarStaticData() {
  radar.scan();
  float* result = radar.getRandarDataBack();
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
String getHTML() {
  return "<!DOCTYPE html><html lang=\"zh\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"><title>radar</title><style>canvas {margin: 20px auto;display: block;}</style><script src=\"https://code.jquery.com/jquery-3.6.0.min.js\"></script></head><body><canvas id=\"can\" width=300 height=300 style=\"transform:rotate(180deg);\"></canvas><script type=\"text/javascript\">var angleDistanceRaw = [];var usingAngle = calculateAngle(angleDistanceRaw);function getRadarData() {jQuery.getJSON( './radarData' , function(data) {usingAngle = calculateAngle(data);getRadarData();});}var CFG = {perDeg: 1,radius: 150,step: 10,maxDistance: 240};function calculateDistance(distance) {if (distance < 0) {return 0;} else if (distance >= CFG.maxDistance) {return CFG.radius;} else {return CFG.radius / CFG.maxDistance * distance;}}function calculateAngle(raw) {var angleList = [];for(var i = 0; i < raw.length; i++) {var index = raw[i].index;var distance = raw[i].distance;for(var j = index; j < 180; j++) {if(distance < 0) {distance = 0;}angleList[j] = distance;}}return angleList;}var can = document.getElementById('can');var ctx = can.getContext('2d');var deg = 0;ctx.strokeStyle = 'rgba(0,255,0,1)';function init() {ctx.fillStyle = 'rgba(0,50,0,1)';ctx.arc(CFG.radius, CFG.radius, CFG.radius, 0, 2 * Math.PI);ctx.fill();var raf = window.requestAnimationFrame(loop);}function loop() {deg = (deg + CFG.perDeg);if (deg == 360) {deg = 0;}cover();drawPosLine();drawRadar(deg);raf = window.requestAnimationFrame(loop);}function cover() {ctx.save();ctx.fillStyle = 'rgba(0,0,0,0.02)';ctx.arc(CFG.radius, CFG.radius, CFG.radius, 0, 2 * Math.PI);ctx.fill();ctx.restore();}function drawPosLine() {ctx.beginPath();ctx.moveTo(CFG.radius, 0);ctx.lineTo(CFG.radius, CFG.radius * 2);ctx.closePath();ctx.stroke();ctx.beginPath();ctx.moveTo(0, CFG.radius);ctx.lineTo(CFG.radius * 2, CFG.radius);ctx.closePath();ctx.stroke();ctx.moveTo(CFG.radius, CFG.radius);ctx.beginPath();ctx.arc(CFG.radius, CFG.radius, CFG.radius / 3 * 2, 0 * Math.PI, 2 * Math.PI);ctx.closePath();ctx.stroke();ctx.moveTo(CFG.radius, CFG.radius);ctx.beginPath();ctx.arc(CFG.radius, CFG.radius, CFG.radius / 3, 0 * Math.PI, 2 * Math.PI);ctx.closePath();ctx.stroke();}function drawRadar(iDeg) {var startAngle = (-2 * CFG.perDeg + iDeg) / 180 * Math.PI;var endAngle = (0+ iDeg) / 180 * Math.PI;var distanceData = usingAngle[iDeg];var distance = calculateDistance(distanceData);if (distanceData < CFG.radius) {ctx.fillStyle = 'rgba(200,0,0,.7)';ctx.beginPath();ctx.moveTo(CFG.radius, CFG.radius);ctx.arc(CFG.radius, CFG.radius, CFG.radius, startAngle, endAngle);ctx.closePath();ctx.fill();ctx.fillStyle = 'rgba(0,200,0,.7)';ctx.beginPath();ctx.moveTo(CFG.radius, CFG.radius);ctx.arc(CFG.radius, CFG.radius, distanceData, startAngle, endAngle);ctx.closePath();ctx.fill();} else {ctx.fillStyle = 'rgba(0,200,0,.7)';ctx.beginPath();ctx.moveTo(CFG.radius, CFG.radius);ctx.arc(CFG.radius, CFG.radius, CFG.radius, startAngle, endAngle);ctx.closePath();ctx.fill();}}init();getRadarData();</script></body></html>";
}
