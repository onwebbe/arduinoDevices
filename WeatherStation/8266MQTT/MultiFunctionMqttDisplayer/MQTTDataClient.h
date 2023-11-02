#ifndef _MQTTDataClient_H__
#define _MQTTDataClient_H__
#include <PubSubClient.h>
#include <LinkedList.h>
#include <ArduinoJson.h>
#include <functional>
#define MQTT_CALLBACK_SIGNATURE std::function<void(char*, uint8_t*, unsigned int)> callback
class MQTTDataClient {
//  private:
//  public:
//    MQTTDataClient();
//    void init();
};
#endif
