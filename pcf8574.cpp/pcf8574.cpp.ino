#include "pcf8574.h"
 
PCF8574 pcf(13,12,0x20);
uint8_t pinsValue;
bool pinValue;
void setup()
{
  Serial.begin(115200);
  pcf.writePins(0x00);//设置所有引脚电平为高
  delay(1000);
  Serial.println("started");
}
 
void loop()
{
  Serial.println("high");
  pcf.setPinHigh(P2);
  delay(5000);
  Serial.println("low");
  pcf.setPinLow(P2);
  delay(5000);
  Serial.println(pcf.readPin(P4));
}