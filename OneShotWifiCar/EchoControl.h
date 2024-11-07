#include <Adafruit_PCF8574.h>


#ifndef _HA_WIFI_CAR_ECHO_CONTROL_H__
#define _HA_WIFI_CAR_ECHO_CONTROL_H__

class EchoControl {
  private:
    int _trigPin;
    int _echoPin;

    void init(Adafruit_PCF8574 *pcf, int trigPin, int echoPin);

    Adafruit_PCF8574 *_pcf;

    bool _trigPinAPCF = false;

  public:
    EchoControl(Adafruit_PCF8574 *pcf, int trigPin, int echoPin);
    void setup();
    float getDistance();

    void setTrigPinAPCF(bool isAPCF);
};
#endif