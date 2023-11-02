/*
 KeyPressed on PIN1
 by Mischianti Renzo <http://www.mischianti.org>

 https://www.mischianti.org/2019/01/02/pcf8575-i2c-digital-i-o-expander-fast-easy-usage/
*/

#include "Arduino.h"
#include "PCF8575.h"

// Set i2c address
PCF8575 pcf8575(0x20);
unsigned long timeElapsed;
long startTime = -1;
long endTime = -1;
bool isHigh = false;
void setup()
{
	Serial.begin(115200);

	pcf8575.pinMode(1, INPUT);

	pcf8575.begin();
}

void loop()
{
	uint8_t val = pcf8575.digitalRead(1);
  Serial.println(val);
	if (val == HIGH && isHigh == false) {
    isHigh = true;
    startTime = millis();
    endTime = -1;
  } else if (val == LOW && isHigh == true) {
    isHigh = false;
    endTime = millis();
  } else if (val == HIGH && isHigh == true && (millis() - startTime) >= 3000) {
    Serial.println(millis() - startTime);
    startTime = -1;
    endTime = -1;
    isHigh = false;
  }
  if (startTime > 0 && endTime > 0) {
    Serial.println(endTime - startTime);
    startTime = -1;
    endTime = -1;
  }
}
