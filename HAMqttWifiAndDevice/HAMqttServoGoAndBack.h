#include <PubSubClient.h>
#include "HAMqttDeviceBase.h"
#include <Servo.h>

#ifndef _HA_MQTT_SERVO_GOANDBACK_H__
#define _HA_MQTT_SERVO_GOANDBACK_H__

class HAMqttServoGoAndBack : public HAMqttDeviceBase {
  private:
    uint8_t _pin;
    bool _isOn = false;

    String _deviceServoGoAndBackStatusChannel_to_server;
    String _deviceServoGoAndBackCommandChannel_from_server;

    int _endAngle = 90;
    int _startAngle = 0;
    bool _isGo = true;
    int _delayTime = 600;

    long _stepTime = 0;

    Servo *_servo;
  
  private:
    void init(PubSubClient* client, uint8_t pin, String deviceName, String haDeviceName, String logChannel);

  public:
    HAMqttServoGoAndBack(PubSubClient* client, uint8_t pin, String deviceName, String logChannel);
    HAMqttServoGoAndBack(PubSubClient* client, uint8_t pin, String deviceName, String haDeviceName, String logChannel);
    
    void setup();
    void doSubscribe();
    void callback(String topicString, String payloadString);
    void beforeReconnect();
    void afterRconnected();
    void loop();
    
  public:
    void publishServoGoBackStatus();
    bool isSwitchOn();
    void setDelayTime(int time);
    void setStartAngle(int angle);
    void setEndAngle(int angle);
};
#endif
