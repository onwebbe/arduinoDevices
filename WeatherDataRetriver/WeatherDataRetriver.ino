#include <ArduinoJson.h>
#include "WeatherDataRetriver.h"
#define DHT22PIN 16
WeatherDataRetriver retriver = WeatherDataRetriver(DHT22PIN);
void setup() {
  Serial.begin(9600);
  Serial.println("start");
  retriver.init();
  Serial.println("start done");
}

void loop() {
  Serial.println("start loop");
  delay(2000);
  Serial.println("start get data");
  String data = retriver.getWeatherData();
  Serial.println("end loop");
  Serial.println();
  Serial.println(data);
  Serial.println();
  Serial.println("end loop");
  
}
