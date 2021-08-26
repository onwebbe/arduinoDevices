/*
 * install Adafruit BMP085 Library firstly
 * MEGA283 A4 - SDA   A5 - SCL VIN - 5v
 */
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp180;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if (!bmp180.begin()){  
    Serial.println("ERROR");///if there is an error in communication  
    while (1) {}
  } 
}

void loop() {
  // put your main code here, to run repeatedly:
  float presure = bmp180.readPressure() / 100.0;
  float altitude = bmp180.readAltitude();
  Serial.println("Presure:" + String(presure) + "hPa");
  Serial.println("Altitude:" + String(altitude) + "m");
  Serial.println();
  delay(1000);
  
}
