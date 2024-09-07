#include <PubSubClient.h>
#include "HAMqttDeviceBase.h"
#ifndef _HA_MQTT_SWITCH_H__
#define _HA_MQTT_SWITCH_H__

class HAMqttSwitch : public HAMqttDeviceBase {
  private:
    uint8_t _pin;
    bool _isOn = false;

    String _deviceSwitchStatusChannel_to_server;
    String _deviceSwitchCommandChannel_from_server;

    int _delaySwitchOff = -1;
    long _delayStart = -1;
  
  private:
    void init(PubSubClient* client, uint8_t pin, String deviceName, String haDeviceName, String logChannel, bool defaultValue);

  public:
    HAMqttSwitch(PubSubClient* client, uint8_t pin, String deviceName, String logChannel);
    HAMqttSwitch(PubSubClient* client, uint8_t pin, String deviceName, String haDeviceName, String logChannel);
    
    void setup();
    void doSubscribe();
    void callback(String topicString, String payloadString);
    void beforeReconnect();
    void afterRconnected();
    void loop();
    
  public:
    void publishSwitchStatus();
    void setDelaySwichOff(int delayMillis);
    bool isSwitchOn();
  
    String getStatusString();
    void restoreStatus();

};
#endif
