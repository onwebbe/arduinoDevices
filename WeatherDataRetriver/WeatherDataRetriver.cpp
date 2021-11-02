#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <ArduinoJson.h>
#include "WeatherDataRetriver.h"
#define DHTType DHT22
#define ADDR 0b0100011


WeatherDataRetriver::WeatherDataRetriver(uint8_t dht22Pin) {
  this->_dht22Pin = dht22Pin;
}
WeatherDataRetriver::~WeatherDataRetriver() {
  delete _dht;
}
void WeatherDataRetriver::init() {
  // Init dht22
//  Serial.println("start dht22");
//  _dht = new DHT(this->_dht22Pin, DHTType);
//  _dht->begin();
//  Serial.println("start dht22 done");

  // Init bmp180
//  Serial.println("start bmp180");
//  _bmp180.begin();
//  Serial.println("start BMP180 done");

  // Init GY30
  Serial.println("start GY30");
  Wire.begin();
  Wire.beginTransmission(ADDR);
  Wire.write(0b00000001);
  Wire.endTransmission();
  Serial.println("start GY30");
}

String WeatherDataRetriver::getWeatherData() {
  
  

//  float t = getTemperature();
//  float h = getHumidity();
//  float p = getPressure();
  int l = getGY30Data();  
  //Print out the Temperature
  
  // object["pressure"] = this->_bmp180->readPressure() / 100.0;
  String resultString = "";
//  String tstr = String(t, 1);
//  String hstr = String(h, 1);
//  String pstr = String(p, 0);
  String lstr = String(l);
//  resultString.concat(tstr);
//  resultString.concat("|");
//  resultString.concat(hstr);
//  resultString.concat("|");
//  resultString.concat(pstr);
//  resultString.concat("|");
  resultString.concat(lstr);
  

  StaticJsonDocument<1024> rootDoc;
  JsonObject object = rootDoc.createNestedObject();
//  object["temperature"] = tstr;
//  object["humidity"] = hstr;
//  object["pressure"] = pstr;
  object["light"] = lstr;
  serializeJson(object, resultString);
  return resultString;
}
float WeatherDataRetriver::getTemperature() {
  Serial.println("start read Temperature");
  double t = this->_dht->readTemperature();
  Serial.println("Temperature: ");
  if (!isnan(t)) {
    //object["temperature"] = t;
    Serial.print(t);
    Serial.print("°C ");
  }
  return t;
}
float WeatherDataRetriver::getHumidity() {
  Serial.println("start read Temperature");
  double h = this->_dht->readHumidity();
  Serial.println("Humidity: ");
  if (!isnan(h)) {
    //object["humidity"] = h;
    Serial.print(h);
    Serial.print("%");
  }
  return h;
}
float WeatherDataRetriver::getPressure() {
  Serial.println("start read Temperature");
  float presure = _bmp180.readPressure() / 100.0;
  Serial.println("Pressure: ");
  Serial.print(presure);
  Serial.println("done read Temperature");
  return presure;
}
int WeatherDataRetriver::getGY30Data() {
  // put your main code here, to run repeatedly:
  int val = 0;
  // reset
  Wire.beginTransmission(ADDR);
  Wire.write(0b00000111);
  Wire.endTransmission();
 
  Wire.beginTransmission(ADDR);
  Wire.write(0b00100000);
  Wire.endTransmission();
  // typical read delay 120ms
  delay(120);
  Wire.requestFrom(ADDR, 2); // 2byte every time
  for (val = 0; Wire.available() >= 1; ) {
    char c = Wire.read();
    //Serial.println(c, HEX);
    val = (val << 8) + (c & 0xFF);
  }
  val = val / 1.2;
  Serial.print("lx: ");
  Serial.println(val);
  Serial.println("OK");
  return val;
}
