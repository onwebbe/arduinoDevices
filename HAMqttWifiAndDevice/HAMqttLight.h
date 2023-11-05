#include <PubSubClient.h>
#include "HAMqttDeviceBase.h"
#ifndef _HA_MQTT_LIGHT_H__
#define _HA_MQTT_LIGHT_H__

class HAMqttLight : public HAMqttDeviceBase {
  private:
    uint8_t _pin;
    bool _isOn = false;
    int _brightness = 255;

    String _deviceSwitchStatusChannel_to_server;
    String _deviceSwitchCommandChannel_from_server;
    String _deviceBrightStatusChannel_to_server;
    String _deviceBrightCommandChannel_from_server;

    int _delaySwitchOff = -1;
    long _delayStart = 0;
 
  public:
    HAMqttLight(PubSubClient* client, uint8_t pin, String deviceName, String logChannel);
    HAMqttLight(PubSubClient* client, uint8_t pin, String deviceName, String haDeviceName, String logChannel);
    void setup();
    void doSubscribe();
    void callback(String topicString, String payloadString);
    void setBrightness(int brightness);
    void publishSwitchStatus();
    void publishBrightStatus();
    void beforeReconnect();
    void afterRconnected();
    // void publishDiscover();
    void loop();
    void setDelaySwichOff(int delayMillis);
};
#endif
