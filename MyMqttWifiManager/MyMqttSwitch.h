#include <PubSubClient.h>

#ifndef _MQTT_SWITCH_H__
#define _MQTT_SWITCH_H__

class MqttSwitch {
  private:
    PubSubClient* _client;
    uint8_t _pin;
    String _channelHeading = "homedevice";
    String _deviceName;
    String _haDeviceName;
    bool _isOn = false;

    String _deviceSwitchStatusChannel_to_server;
    String _deviceSwitchCommandChannel_from_server;
    String _deviceDataChannel_to_server;
    String _deviceCommandChannel_from_server;
    String _deviceAutoDiscoverChannel_to_server;
    String _deviceAutoDiscoverData;

    String _deviceLogChannel_to_server;

    long _recordTime;

    int _debugLevel = 1; // 0 - nodebug 1 - error   2 - info    3 - debug

    int _delaySwitchOff = -1;
    long _delayStart = 0;
 
  public:
    MqttSwitch(PubSubClient* client, uint8_t pin, String deviceName, String logChannel);
    MqttSwitch(PubSubClient* client, uint8_t pin, String deviceName, String haDeviceName, String logChannel);
    void setup();
    void doSubscribe();
    void callback(String topicString, String payloadString);
    void setHeading(String heading);
    void publishSwitchStatus();
    void beforeReconnect();
    void afterRconnected();
    void publishDiscover();
    void log(int level, String s);
    void loop();
    void setDebugLevel(int debugLevel);
    void setDelaySwichOff(int delayMillis);

  public:
    int ERROR = 1;
    int INFO = 2;
    int DEBUG = 3;
};
#endif
