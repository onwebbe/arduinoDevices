#include <PubSubClient.h>
#include <LinkedList.h>
#ifndef _MQTT_SENSOR_H__
#define _MQTT_SENSOR_H__
class MqttSensorConfig {
  private:
    String _unit;
    String _nodeName;
    String _data;
  public:
    MqttSensorConfig(String unit, String nodeName);
    String getUnit();
    String getNodeName();
    String getTemplate();
    String getData(); 
    void setData(String data);
};
class MqttSensor {
  private:
    PubSubClient* _client;
    uint8_t _pin;
    String _channelHeading = "homedevice";
    String _deviceName;
    String _haDeviceName;
    bool _isOn = false;

    LinkedList<MqttSensorConfig*>* _sensorDataConfigList = new LinkedList<MqttSensorConfig*>();
    String _deviceSensorStatusChannel_to_server;
    String _deviceDataChannel_to_server;
    String _deviceCommandChannel_from_server;
    String _deviceAutoDiscoverChannel_to_server;
    String _deviceAutoDiscoverData;

    String _deviceLogChannel_to_server;

    long _recordTime;
    long _publishDataTime;

    int _debugLevel = 1; // 0 - nodebug 1 - error   2 - info    3 - debug

    int _publishGap = 1000;
 
  public:
    MqttSensor(PubSubClient* client, uint8_t pin, String deviceName, String logChannel);
    MqttSensor(PubSubClient* client, uint8_t pin, String deviceName, String haDeviceName, String logChannel);
    void setup();
    void doSubscribe();
    void callback(String topicString, String payloadString);
    void setHeading(String heading);
    void publishSensorStatus();
    void beforeReconnect();
    void afterRconnected();
    void publishDiscover();
    void log(int level, String s);
    void loop();
    void setDebugLevel(int debugLevel);
    void addSensorConfig(MqttSensorConfig* configData);
  private:
    void publishData();
  public:
    int ERROR = 1;
    int INFO = 2;
    int DEBUG = 3;
};
#endif
