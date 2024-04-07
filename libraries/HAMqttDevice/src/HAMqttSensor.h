#include <PubSubClient.h>
#include "HAMqttDeviceBase.h"
#include <LinkedList.h>
#ifndef _HA_MQTT_SENSOR_H__
#define _HA_MQTT_SENSOR_H__
class HAMqttSensorConfig{
  private:
    String _unit;
    String _nodeName;
    String _data;
    int _type = 1;  // 1: string, 2: double
  public:
    HAMqttSensorConfig(String unit, String nodeName);
    String getUnit();
    String getNodeName();
    String getTemplate();
    String getData();
    void setData(String data);
    void setType(int type);
    int getType();
};
class HAMqttSensor : public HAMqttDeviceBase {
  private:
    LinkedList<HAMqttSensorConfig*>* _sensorDataConfigList = new LinkedList<HAMqttSensorConfig*>();
    String _deviceSensorStatusChannel_to_server;

    long _publishDataTime;

    int _publishGap = 1000;
 
  public:
    HAMqttSensor(PubSubClient* client, String deviceName, String logChannel);
    HAMqttSensor(PubSubClient* client, String deviceName, String haDeviceName, String logChannel);
    void setup();
    void doSubscribe();
    void callback(String topicString, String payloadString);
    void beforeReconnect();
    void afterRconnected();
    void publishDiscover();
    void loop();
    
  public:
    void addSensorConfig(HAMqttSensorConfig* configData);
    void publishData();
    void setPublishGap(int time);
};
#endif
