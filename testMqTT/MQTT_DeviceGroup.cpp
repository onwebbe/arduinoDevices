#include "MQTT_DeviceGroup.h"
#include "MQTT_Device.h"
#include "Utils_8266.h"

MQTT_DeviceGroup::MQTT_DeviceGroup(String deviceName, String type) {
  _deviceName = deviceName;
  _type = type;
  
  String discoverHead = Utils_8266::DISCOVER_HEAD;
  _HEADBEAT_CHANNEL = discoverHead + "/"+ _type + "/" + _deviceName + "/state";
  _SEND_CHANNEL = discoverHead + "/"+ _type + "/" + _deviceName + "/data";
  _RECEIVE_CHANNEL = _SEND_CHANNEL;
  _DISCOVER_CHANNEL_BASE = discoverHead + "/"+ _type + "/" + _deviceName;
  _GROUPED_CONFIG_DEVICEList = LinkedList<MQTT_Device*>();
}
void MQTT_DeviceGroup::addConfig(String deviceClass, String name, String label, String unit, String dataField) {
  MQTT_Device* device = new MQTT_Device(deviceClass, name, label, unit, dataField, _SEND_CHANNEL);
  _GROUPED_CONFIG_DEVICEList.add(device);
}
void MQTT_DeviceGroup::publishConfigs(PubSubClient* client) {
  MQTT_Device* device;
  for(int i = 0; i < _GROUPED_CONFIG_DEVICEList.size(); i++){
    // Get device from list
    device = _GROUPED_CONFIG_DEVICEList.get(i);
    String configJSON = device->getConfigJSON();
    Serial.println("publish discover " + device->getName());
    Serial.println(configJSON.c_str());
    client->publish((_DISCOVER_CHANNEL_BASE + "_" + device->getName() + "/config").c_str(), configJSON.c_str());
  }
}
