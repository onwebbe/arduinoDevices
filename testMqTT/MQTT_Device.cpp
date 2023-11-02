#include "MQTT_Device.h"
MQTT_Device::MQTT_Device(String deviceClass, String name, String label, String unit, String dataField, String channel) {
  _deviceName = name;
  _unit = unit;
  _label = label;
  _dataField = dataField;
  _deviceClass = deviceClass;
  _channel = channel;
}
String MQTT_Device::getConfigJSON() {
  return "{\"device_class\": \"" + _deviceClass + "\", \"name\": \"" + _deviceName + "\", \"state_topic\": \"" + _channel + "\", \"unit_of_measurement\": \"" + _unit + "\", \"value_template\": \"{{ value_json." + _dataField + "}}\" }";
}
