#include <PubSubClient.h>

#ifndef _HA_MQTT_DEVICEBASE_H__
#define _HA_MQTT_DEVICEBASE_H__

class HAMqttDeviceBase {
  protected:
    PubSubClient* _client;
    uint8_t _pin;
    String _channelHeading = "homedevice";
    String _deviceName;
    String _haDeviceName;
    String _type;

    String _deviceDataChannel_to_server;
    String _deviceCommandChannel_from_server;
    String _deviceAutoDiscoverChannel_to_server;
    String _deviceAutoDiscoverData;

    String _deviceLogChannel_to_server;

    long _recordTime;

    long _discoverTimeGap = 60000;

    int _debugLevel = 1; // 0 - nodebug 1 - error   2 - info    3 - debug

 
  public:
    HAMqttDeviceBase(PubSubClient* client, String type, String deviceName, String logChannel);
    HAMqttDeviceBase(PubSubClient* client, String type, String deviceName, String haDeviceName, String logChannel);
    void virtual setup();
    void virtual doSubscribe();
    void virtual callback(String topicString, String payloadString);
    void setHeading(String heading);
    void virtual beforeReconnect();
    void virtual afterRconnected();
    void virtual publishDiscover();
    void log(int level, String s);
    void virtual loop();
    void setDebugLevel(int debugLevel);
    void setDiscoverTimeGap(long gap);

  protected:
    void processCallbackCommandCommon(String topicString, String payloadString);
  public:
    const int ERROR = 1;
    const int INFO = 2;
    const int DEBUG = 3;
};
#endif
