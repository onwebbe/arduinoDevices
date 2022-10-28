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

    String _HEADBEAT_CHANNEL;
    String _SEND_CHANNEL;
    String _RECEIVE_CHANNEL;
    String _DISCOVER_CHANNEL_BASE;

    bool _isSuccessConnected;
  public:
    MQTT_8266(String deviceName, String type);
    ~MQTT_8266();
    void init(String host, int port, String clientId, String clientPwd);
    void heartBeat();
    void reconnect();
    void processInLoop();
    void publish_data(String s);
    void publish_binary_sensor_data(String s);
  private:
    void publishContentTemperature();
    void publishContentHumidity();
    void publishContentPressure();
    void publishContentLight();
    void publishContentWindSpeed();
    void publishContentRain();
};
class MQTT_Device {
  private:
    String _deviceName;
    String _unit;
    String _label;
    String _dataField; //sample value_json.temperature
    String _deviceClass;
    String _channel;
  public:
    MQTT_Device(String deviceClass, String name, String label, String unit, String dataField, String channel);
    String getName();
    String getLabel();
    String getUnit();
    String getDataField();
    String getConfigJSON();
    String getChannel();
    String getDeviceClass();
};
class MQTT_DeviceGroup {
  private:
    String _type;
    String _deviceName;
    String _HEADBEAT_CHANNEL;
    String _SEND_CHANNEL;
    String _RECEIVE_CHANNEL;
    String _DISCOVER_CHANNEL_BASE;
    LinkedList<MQTT_Device*> _GROUPED_CONFIG_DEVICEList;

    String _MQTT_HOST;
    int _MQTT_PORT;
    String _MQTT_CLIENT_ID;
    String _MQTT_CLIENT_PWD;
  public:
    MQTT_DeviceGroup(String type, String deviceName);
    void addConfig(MQTT_Device device);
    void addConfig(String deviceClass, String name, String label, String unit, String dataField);
    void init(String host, int port, String clientId, String clientPwd);
    void publishConfigs();
};
#endif
