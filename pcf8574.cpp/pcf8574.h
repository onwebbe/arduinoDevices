#ifndef PCF8574_H__
#define PCF8574_H__
#include <Arduino.h>
#include <Wire.h>
 
//A0=0
//A1=0
//A2=0
#define PCF8574_ADDR  0X20
#define PCF8574A_ADDR 0X70
 
#define P0 0X00
#define P1 0X01
#define P2 0X02
#define P3 0X03
#define P4 0X04
#define P5 0X05
#define P6 0X06
#define P7 0X07
 
class PCF8574
{
public:
 
  PCF8574(uint8_t sda_pin, uint8_t scl_pin, uint8_t addr);
  PCF8574(uint8_t sda_pin, uint8_t scl_pin);
  void writePins(uint8_t pinsValue);
  void setPinHigh(uint8_t pin);
  void setPinLow(uint8_t pin);
  void setPinInvert(uint8_t pin);
  uint8_t readPins(void);
  bool readPin(uint8_t pin);
  
private:
  uint8_t _addr;   // I2C communication address of PCF8574
};
 
#endif