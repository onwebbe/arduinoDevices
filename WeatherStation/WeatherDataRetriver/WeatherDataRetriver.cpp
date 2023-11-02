#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <ArduinoJson.h>
#include "HorerSensor.h"
#include "WeatherDataRetriver.h"
#include <WeatherDisplay.h>
#define DHTType DHT22
#define ADDR 0b0100011


WeatherDataRetriver::WeatherDataRetriver(int dht22Pin, int hornerPin, int rainDropPin) {
  this->_dht22Pin = dht22Pin;
  this->_hornerSensorPin = hornerPin;
  this->_rainDropPin = rainDropPin;
}
WeatherDataRetriver::~WeatherDataRetriver() {
  delete _dht;
  delete _horner;
}
void WeatherDataRetriver::init() {
  // display
  WeatherDisplay weatherDisplay = WeatherDisplay();
  
  _weatherDisplay = &weatherDisplay;
  _weatherDisplay->init();
  // Init dht22
  Serial.println("start dht22");
  _dht = new DHT(this->_dht22Pin, DHTType);
  _dht->begin();
  Serial.println("start dht22 done");

  // Init bmp180
  Serial.println("start bmp180");
  _bmp180.begin();
  Serial.println("start BMP180 done");

  // Init GY30
  Serial.println("start GY30");
  Wire.begin();
  Wire.beginTransmission(ADDR);
  Wire.write(0b00000001);
  Wire.endTransmission();
  Serial.println("start GY30");

  // Init RainDrop
  Serial.println("start RainDrop");
  pinMode(_rainDropPin, INPUT);
  
  // Init Horner wind speed
  _horner = new HorerSensor(this->_hornerSensorPin);
  _horner->init();
}

String WeatherDataRetriver::getWeatherData() {
  float t = getTemperature();
  float h = getHumidity();
  float p = getPressure();
  int l = getGY30Data();  
  boolean r = isRain();
  float w = getWindSpeed();
  _weatherDisplay->displayData(t, h, l, w, p, r);
  //Print out the Temperature
  
  // object["pressure"] = this->_bmp180->readPressure() / 100.0;
  String resultString = "";
  String tstr = String(t, 1);
  String hstr = String(h, 0);
  String pstr = String(p, 0);
  String lstr = String(l);
  String wstr = String(w);
  Serial.println("r?");
  Serial.println(r == true ? "true":"false");
  String rstr = (r == true ? "true":"false");
//  resultString.concat(tstr);
//  resultString.concat("|");
//  resultString.concat(hstr);
//  resultString.concat("|");
//  resultString.concat(pstr);
//  resultString.concat("|");
//  resultString.concat(lstr);
  

  StaticJsonDocument<1024> rootDoc;
  JsonObject object = rootDoc.createNestedObject();
  object["temperature"] = tstr;
  object["humidity"] = hstr;
  object["pressure"] = pstr;
  object["light"] = lstr;
  object["wind"] = wstr;
  object["rain"] = rstr;
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
  Serial.println("start read Humidity");
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
  Serial.println("start read Pressure");
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
float WeatherDataRetriver::getWindSpeed() {
  return _horner->getAverageRPM();
  // return 0;
}
boolean WeatherDataRetriver::isRain() {
  int isRainDrop = digitalRead(_rainDropPin);
  Serial.println("isRain:");
  Serial.println(isRainDrop);
  return isRainDrop == 1 ? false: true;
  // return 0;
}
