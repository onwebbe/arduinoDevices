#include <WiFiManager.h>
#include "Arduino.h"

#ifndef __Utils_8266_H
#define __Utils_8266_H

class Utils_8266 {
  public:
    static WiFiManager* _wifiManager;
    static String DISCOVER_HEAD;
  public:
    static void setupWiFiManager(WiFiManager* wifiManager);
    static void resetNetworkSettingsWhenFailed();
    static void restartOnly();
    static String readFile(const char * path);
    static void writeFile(const char * path, const char * message);
    static void Split(String zifuchuan,String fengefu,String result[]);
};
#endif
