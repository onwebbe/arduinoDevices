#include <Arduino.h>
#include <Wire.h>
static const uint8_t AHT10_ADDR = 0x38;
void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  Wire.beginTransmission(AHT10_ADDR);
  Wire.write(0xE1);
  Wire.write(0x08);
  Wire.write(0x00);
  if(Wire.endTransmission() == 0) {
    Serial.println("Fount AHT10");
  } else {
    Serial.println("Failed AHT10");
    delay(5000);
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(getTemperature());
  Serial.println(getHumidity());
  delay(3000);

}

bool measure(float *temp, float *hum) {
  uint8_t data[6];
  uint32_t d;

  Wire.beginTransmission(AHT10_ADDR);
  Wire.write(0xAC);
  Wire.write(0x33);
  Wire.write(0x00);
  if (Wire.endTransmission())
    return false;
  delay(75);
  if ((Wire.requestFrom(AHT10_ADDR, (uint8_t)6) != 6) || (Wire.readBytes(data, 6) != 6))
    return false;
  if (temp) {
    d = ((uint32_t)(data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];
    *temp = (float)d * 200 / 1048576 - 50;
  }
  if (hum) {
    d = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | (data[3] >> 4);
    *hum = (float)d * 100 / 1048576;
  }
  return true;
}

float getTemperature() {
  float result;

  if (! measure(&result, NULL))
    result = NAN;
  return result;
}

float getHumidity() {
  float result;

  if (! measure(NULL, &result))
    result = NAN;
  return result;
}