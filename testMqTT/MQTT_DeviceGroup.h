#include "MQTT_Device.h"

#include "Arduino.h"
#include <LinkedList.h>
#include <PubSubClient.h>
#include <FS.h>
/*******************
MQTT_8266.h
MQTT driver
*******************/
#ifndef _MQTT_8266_GROUP_H__
#define _MQTT_8266_GROUP_H__
class MQTT_DeviceGroup {
  private:
    String _type;
    String _deviceName;
    LinkedList<MQTT_Device*> _GROUPED_CONFIG_DEVICEList;
    String _HEADBEAT_CHANNEL;
    String _SEND_CHANNEL;
    String _RECEIVE_CHANNEL;
    String _DISCOVER_CHANNEL_BASE;

  public:
    MQTT_DeviceGroup(String type, String deviceName);
    void addConfig(MQTT_Device device);
    void addConfig(String deviceClass, String name, String label, String unit, String dataField);
    void publishConfigs(PubSubClient* client);
};
#endif
