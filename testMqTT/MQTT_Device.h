#include "Arduino.h"
#include <LinkedList.h>
#include <PubSubClient.h>
#include <FS.h>
/*******************
MQTT_8266.h
MQTT driver
*******************/
#ifndef _MQTT_DEVICE_H__
#define _MQTT_DEVICE_H__
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
#endif
