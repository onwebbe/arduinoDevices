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
    static String readFile(const char * path);
    static void writeFile(const char * path, const char * message);
    static void Split(String zifuchuan,String fengefu,String result[]);
    static String _t;
    static String _h;
    static String _l;
    static String _p;
    static String _w;
    static String _r;
};

#endif
