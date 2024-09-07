#include "pcf8574.h"
 
PCF8574::PCF8574(uint8_t sda_pin, uint8_t scl_pin, uint8_t addr)
{
  _addr = addr;
  Wire.begin(sda_pin, scl_pin);
}
PCF8574::PCF8574(uint8_t sda_pin, uint8_t scl_pin)
{
  _addr = PCF8574_ADDR;
  Wire.begin(sda_pin, scl_pin);
}
void PCF8574::writePins(uint8_t pinsValue)
{
  Wire.beginTransmission(_addr);
  Wire.write(pinsValue);
  Wire.endTransmission();
}
void PCF8574::setPinHigh(uint8_t pin)
{
  uint8_t pinsValue;
  pinsValue = this->readPins();
  pinsValue |= (1 << pin);
  this->writePins(pinsValue);
}
void PCF8574::setPinLow(uint8_t pin)
{
  uint8_t pinsValue;
  pinsValue = this->readPins();
  pinsValue &= ~(1 << pin);
  this->writePins(pinsValue);
}
void PCF8574::setPinInvert(uint8_t pin)
{
  uint8_t pinsValue;
  pinsValue = this->readPins();
  if(pinsValue&(1<<pin))this->setPinLow(pin);
  else this->setPinHigh(pin);
}
uint8_t PCF8574::readPins(void)
{
  uint8_t pinsValue;
  Wire.requestFrom(_addr, (uint8_t)1);
  pinsValue = Wire.read();
  return pinsValue;
}
bool PCF8574::readPin(uint8_t pin)
{
  uint8_t pinsValue;
  Wire.requestFrom(_addr, (uint8_t)1);
  pinsValue = Wire.read();
  if(pinsValue&(1<<pin))
  {
    return HIGH;    
  }
  else 
  {
    return LOW;
  }
}