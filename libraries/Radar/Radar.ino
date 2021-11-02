#include "Radar.h"
#include <ArduinoJson.h>
Radar radar(25, 26, 27);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  radar.scan();
  float* result = radar.getRandarDataGo();
  String jsonResult;
//  serializeJson(doc, jsonResult);
  Serial.println(jsonResult);
  int resultSize = sizeof(result);
  Serial.println(String(resultSize));
//  for(int i = 0; i <= 18; i++) {
//    Serial.println("angle:" + String(i * 10) + " distance:" + String(result[i]) + "cm");
//  }
  generateJSON(result);
}
void generateJSON(float* rawArray) {
  StaticJsonDocument<1024> rootDoc;
  for(int i = 0; i <= 18; i++) {
    JsonObject object = rootDoc.createNestedObject();
    object["index"] = i * 10;
    float distance = rawArray[i];
    // Serial.println("distance" + String(distance));
    object["distance"] = distance;
  }
  serializeJson(rootDoc, Serial);
}
void writeFile() {
  String s = "<!DOCTYPE html><html lang=\"zh\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1.0\"><meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"><title>radar</title><style>canvas {margin:20px auto;display:block;}</style></head><body><canvas id=\"can\" width=300 height=300 style=\"transform:rotate(180deg);\"></canvas><script type=\"text/javascript\">var CFG = {perDeg:1,radius:150,step:10,maxDistance:240};function calculateDistance(distance,max) {if (distance >= CFG.maxDistance) {return max;} else {return distance * max / CFG.maxDistance;}}function calculateAngle(raw) {var angleList = [];for(var i = 0; i < raw.length; i++) {var index = raw[i].index;var distance = raw[i].distance;for(var j = index; j < 180; j++) {angleList[j] = distance;}}return angleList;}var angleDistanceRaw = [{index:0,distance:240},{index:10,distance:240},{index:20,distance:240},{index:30,distance:240},{index:40,distance:240},{index:50,distance:240},{index:60,distance:240},{index:70,distance:10},{index:80,distance:10},{index:90,distance:240},{index:100,distance:240},{index:110,distance:30},{index:120,distance:30},{index:130,distance:240},{index:140,distance:240},{index:150,distance:240},{index:160,distance:240},{index:170,distance:240}];var usingAngle = calculateAngle(angleDistanceRaw);var can = document.getElementById('can');var ctx = can.getContext('2d');var deg = 0;ctx.strokeStyle = 'rgba(0,255,0,1)';function init() {ctx.fillStyle = 'rgba(0,50,0,1)';ctx.arc(CFG.radius,CFG.radius,CFG.radius,0,2 * Math.PI);ctx.fill();var raf = window.requestAnimationFrame(loop);}function loop() {deg = (deg + CFG.perDeg);if (deg == 360) {deg = 0;}cover();drawPosLine();drawRadar(deg);raf = window.requestAnimationFrame(loop);}function cover() {ctx.save();ctx.fillStyle = 'rgba(0,0,0,0.02)';ctx.arc(CFG.radius,CFG.radius,CFG.radius,0,2 * Math.PI);ctx.fill();ctx.restore();}function drawPosLine() {ctx.beginPath();ctx.moveTo(CFG.radius,0);ctx.lineTo(CFG.radius,CFG.radius * 2);ctx.closePath();ctx.stroke();ctx.beginPath();ctx.moveTo(0,CFG.radius);ctx.lineTo(CFG.radius * 2,CFG.radius);ctx.closePath();ctx.stroke();ctx.moveTo(CFG.radius,CFG.radius);ctx.beginPath();ctx.arc(CFG.radius,CFG.radius,CFG.radius / 3 * 2,0 * Math.PI,2 * Math.PI);ctx.closePath();ctx.stroke();ctx.moveTo(CFG.radius,CFG.radius);ctx.beginPath();ctx.arc(CFG.radius,CFG.radius,CFG.radius / 3,0 * Math.PI,2 * Math.PI);ctx.closePath();ctx.stroke();}function drawRadar(iDeg) {var startAngle = (-2 * CFG.perDeg + iDeg) / 180 * Math.PI;var endAngle = (0+ iDeg) / 180 * Math.PI;var distanceData = usingAngle[iDeg];var distance = calculateDistance(distanceData);if (distanceData < CFG.maxDistance) {ctx.fillStyle = 'rgba(200,0,0,.7)';ctx.beginPath();ctx.moveTo(CFG.radius,CFG.radius);ctx.arc(CFG.radius,CFG.radius,CFG.radius,startAngle,endAngle);ctx.closePath();ctx.fill();ctx.fillStyle = 'rgba(0,200,0,.7)';ctx.beginPath();ctx.moveTo(CFG.radius,CFG.radius);ctx.arc(CFG.radius,CFG.radius,distanceData,startAngle,endAngle);ctx.closePath();ctx.fill();} else {ctx.fillStyle = 'rgba(0,200,0,.7)';ctx.beginPath();ctx.moveTo(CFG.radius,CFG.radius);ctx.arc(CFG.radius,CFG.radius,CFG.radius,startAngle,endAngle);ctx.closePath();ctx.fill();}}init();</script></body></html>";
}
