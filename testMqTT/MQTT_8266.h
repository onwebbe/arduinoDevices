#include "MQTT_DeviceGroup.h"
#include "MQTT_Device.h"

#include "Arduino.h"
#include <LinkedList.h>
#include <PubSubClient.h>
#include <FS.h>
/*******************
MQTT_8266.h
MQTT driver
*******************/
#ifndef _MQTT_8266_H__
#define _MQTT_8266_H__

class MQTT_8266 {
  public:
  private:
    String _type;
    String _deviceName;
    String _discoverHead;
    String _MQTT_HOST;
    int _MQTT_PORT;
    String _MQTT_CLIENT_ID;
    String _MQTT_CLIENT_PWD;

    bool _isSuccessConnected;

    LinkedList<MQTT_DeviceGroup*> _mqtt_DeviceGroup;

    PubSubClient* _client;
  public:
    MQTT_8266(String deviceName, String type);
    ~MQTT_8266();
    void init(String host, int port, String clientId, String clientPwd);
    void heartBeat();
    void reconnect();
    void processInLoop();
    void publish_data(String s);
    void addDeviceGroup(MQTT_DeviceGroup* group);
    PubSubClient* getClient();
    void publish_config();
};

#endif
