#include "AHT10_8266.h"
#include "BMP180_8266.h"
#include "HornerSensor_8266.h"
#include "Rain_8266.h"
#include "GY30_8266.h"

AHT10_8266 *ath10 = new AHT10_8266();

BMP180_8266 *bmp180 = new BMP180_8266();

HorerSensor_8266 *horner = new HorerSensor_8266(14);

Rain_8266 *rain = new Rain_8266(8);

GY30_8266 *gy30 = new GY30_8266();

void setup() {
  // put your setup code here, to run once:
  ath10->setup();

  bmp180->setup();

  horner->setup();

  rain->setup();

  gy30->setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(ath10->getTemperature());
  Serial.println(ath10->getHumidity());

  Serial.println(bmp180->getPressure());

  Serial.println(horner->getAverageRPM());

  Serial.println(rain->isRain());

  Serial.println(gy30->getGY30Data());

  delay(1000);
}
