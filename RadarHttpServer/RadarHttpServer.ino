#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>
#include <Radar.h>

const char* ssid = "pankey_asus";
const char* password = "1234554321";
Radar radar(25, 26, 27);
WebServer server(80);

const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp32!");
  digitalWrite(led, 0);
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
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
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

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/radar", []() {
    server.send(200, "text/html", getHTML());
  });

  server.on("/radarData", []() {
    String radarJSONString = getRadarData();
    server.send(200, "text/json", radarJSONString);
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
String getRadarData() {
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
