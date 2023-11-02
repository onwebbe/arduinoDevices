#include "Arduino.h"
#include <LinkedList.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
/*******************
HorerSensor.h
HorerSensor driver
*******************/
#ifndef _WIFI_NETWORK_8266_H__
#define _WIFI_NETWORK_8266_H__
class WIFI_NETWORK_8266 {
  public:
  private:
    WiFiManager* _wifiManager;
    String _client_id;
    String _client_pwd;
    String _wifiName;
  public:
    WIFI_NETWORK_8266(String wifiName, String defaultClientId);
    ~WIFI_NETWORK_8266();
    bool init();
    void resetNetwork();
    String getClientId();
    String getClientPassword();
    WiFiManager* getWiFiManager();
  private:
    static void saveConfigCallbackWifiManager();
};
#endif
