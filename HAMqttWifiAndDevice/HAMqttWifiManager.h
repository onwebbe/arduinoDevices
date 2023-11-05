#include "SSD1306Wire.h"
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

#ifndef _HA_MQTT_WIFI_MANAGER_H__
#define _HA_MQTT_WIFI_MANAGER_H__

class HAMqttWifiManager {
  private:
    WiFiManager* _wm;
    String _networkFileName = "/network.txt";
    String _client_id;
    String _client_password;
    String _client_ip;
    String _client_port = "9013";
    String _device_name;

  private:
    std::function<void()> _notConnectedCallBack;
    std::function<void(String)> _connectedCallback;
    std::function<void()> _startWifiCallback;
  private:
    bool shouldSaveConfig = false;
    String readFile(const char * path);
    void writeFile(const char * path, const char * message);
    void split(String zifuchuan,String fengefu,String result[]);
    
  public:
    HAMqttWifiManager();
    HAMqttWifiManager(String deviceName, String clientIP);
    void saveConfigToFile();
    void setDeviceName(String deviceName);
    void setClientIP(String clientIP);
    void setClientId(String clientId);
    void setClientPassword(String clientPassword);
    void setClientPort(String port);
    String getClientID();
    String getClientPassword();
    String getClientIP();
    String getClientPort();
    void setup();
    void reconnect();
    void setNotConnectedCallback( std::function<void()> func );
    void setConnectSuccessfulCallback( std::function<void(String)> func );
    void setStartWifiNetworkCallback( std::function<void()> func );
    void resetNetworkSettingsWhenFailed();
    bool loop();
};
#endif
