#include <Arduino.h>
#include <Wire.h>
#include "AHT10_8266.h"
#include "BMP180_8266.h"
#include "Displayer.h"
#include <Adafruit_AHTX0.h>

bool isAHT10 = false;
bool isBMP180 = false;
long timeGap = 500;
long timeGapFace = 500;
long lastTime = 0;
long lastTimeFace = 0;
Displayer displayer;
Adafruit_AHTX0 aht;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Wire.begin(0x38);
  // if (! aht.begin()) {
  if (! AHT10_8266::begin()) {
    Serial.println(F("AHT10 not detected!"));
  } else {
    isAHT10 = true;
  }
  if (!BMP180_8266::begin()) {
    Serial.println(F("BMP180 not detected!"));
  } else {
    isBMP180 = true;
  }
  Serial.println("1");
  displayer.init();
  Serial.println("2");
  lastTime = millis();
  Serial.println("3");
}
float temperatureValue = 0;
float humidityValue = 0;
float pressure = 0;
void loop() {
  // put your main code here, to run repeatedly:
  if ((millis() - lastTime) >= timeGap || (millis() - lastTime) < 0) {
    Serial.println("5");
    if (isAHT10) {
      // sensors_event_t humidity, temp;
      // aht.getEvent(&humidity, &temp);
      // temperatureValue = temp.temperature;
      // humidityValue = humidity.relative_humidity;
      temperatureValue = AHT10_8266::getTemperature();
      humidityValue = AHT10_8266::getHumidity();
      Serial.print(temperatureValue);
      Serial.println(" C");
      Serial.println();
  
      Serial.print(humidityValue);
      Serial.println(" %");
      Serial.println();
    }
    if (isBMP180) {
      pressure = BMP180_8266::getPressure();
      Serial.print(pressure);
      Serial.println(" hPa");
      Serial.println();
    }
    lastTime = millis();
  }
  if ((millis() - lastTimeFace) >= timeGap || (millis() - lastTimeFace) < 0) {
    int isLaugh = random(0, 10);
    displayer.setLaugh(isLaugh>5);
    lastTimeFace = millis();
  }
  displayer.setWeatherData(temperatureValue, humidityValue, pressure);
  displayer.loop();
}
