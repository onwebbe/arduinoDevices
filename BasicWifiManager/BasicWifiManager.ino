#define BLINKER_PRINT Serial
#define BLINKER_WIFI
#define BLINKER_WITHOUT_SSL
#define BLINKER_DUEROS_LIGHT

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ESP8266WiFi.h> 
#include <Blinker.h>

#define LIGHT_PIN 16
// 新建组件对象
BlinkerButton Button1("btn-light");
BlinkerNumber Number1("num-input");
char auth[] = "191eeef8c5a1";
char BLINK_TOKEN[20] = "";
boolean shouldSaveConfig = false;
int counter = 0;
int isLightOn = 0;
boolean isStart = false;
void button1_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    isLightOn = !isLightOn;
    digitalWrite(LIGHT_PIN, isLightOn);
    BLINKER_LOG("Light is:", isLightOn == true ? "ON":"OFF");
    if (isLightOn) {
      Button1.text("已开灯");
      Button1.color("Green");
      Button1.print();
    } else {
      Button1.text("已关灯");
      Button1.color("DarkRed");
      Button1.print();
    }
}

// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
    counter++;
    Number1.print(counter);
}

void duerQuery(int32_t queryCode) {
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("DUER Query All");
            BlinkerDuerOS.powerState(isLightOn ? "on" : "off");
            BlinkerDuerOS.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("DUER Query Power State");
            BlinkerDuerOS.powerState(isLightOn ? "on" : "off");
            BlinkerDuerOS.print();
            break;
        default :
            BlinkerDuerOS.powerState(isLightOn ? "on" : "off");
            BlinkerDuerOS.print();
            break;
    }
}
void duerPowerState(const String & state) {
    BLINKER_LOG("need set power state: ", state);
    if (state == BLINKER_CMD_ON) {
      digitalWrite(LIGHT_PIN, HIGH);
      BlinkerDuerOS.powerState("on");
      BlinkerDuerOS.print();
      BLINKER_LOG("Light is: ON");
      isLightOn = 1;
    }
    else if (state == BLINKER_CMD_OFF) {
      digitalWrite(LIGHT_PIN, LOW);
      BlinkerDuerOS.powerState("off");
      BlinkerDuerOS.print();
      BLINKER_LOG("Light is: OFF");
      isLightOn = 0;
    }
    if (isLightOn) {
      Button1.text("已开灯");
      Button1.color("Green");
      Button1.print();
    } else {
      Button1.text("已关灯");
      Button1.color("DarkRed");
      Button1.print();
    }
}

void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}
void setup() {
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LIGHT_PIN, OUTPUT);
    digitalWrite(LIGHT_PIN, LOW);
    // put your setup code here, to run once:
    Serial.begin(115200);
    
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    wm.resetSettings();

    wm.setSaveConfigCallback(saveConfigCallback);

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result
    WiFiManagerParameter custom_api_token("apikey", "API token", auth, 20);
    wm.addParameter(&custom_api_token);
    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("AutoConnectAP-2","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
        Serial.println(custom_api_token.getValue());
        Serial.println("--------get wifi ssid--------");
        Serial.println(wm.getWiFiSSID());
        Serial.println(wm.getWiFiPass());

        #if defined(BLINKER_PRINT)
//          BLINKER_DEBUG.stream(BLINKER_PRINT);
          BLINKER_DEBUG.stream(Serial);
          BLINKER_DEBUG.debugAll();
        #endif
        // 初始化有LED的IO
//        pinMode(LED_BUILTIN, OUTPUT);
//        digitalWrite(LED_BUILTIN, HIGH);
        
        digitalWrite(LED_BUILTIN, LOW);
        pinMode(LED_BUILTIN, INPUT);
        
        // 初始化blinker  (char*)str
        Blinker.begin(custom_api_token.getValue(), (char*)wm.getWiFiSSID().c_str(), (char*)wm.getWiFiPass().c_str());
        Blinker.attachData(dataRead);
        Button1.attach(button1_callback);
        Button1.text("已关灯");
        Button1.color("DarkRed");
        Button1.print();
    
        BlinkerDuerOS.attachPowerState(duerPowerState);
        BlinkerDuerOS.attachQuery(duerQuery);
        isStart = true;
    }

}

void loop() {
    // put your main code here, to run repeatedly:   
    if (isStart == true) {
      Blinker.run();
    }
}
