
#define BLINKER_PRINT Serial
#define BLINKER_WIFI
#define BLINKER_WITHOUT_SSL
#define LED_BUILTIN 4
#define BLINKER_DUEROS_LIGHT
#define BLINKER_MIOT_LIGHT
#include <Blinker.h>

char auth[] = "191eeef8c5a1";
char ssid[] = "pankey_asus";
char pswd[] = "1234554321";

  // 新建组件对象
  BlinkerButton Button1("btn-abc");
  BlinkerNumber Number1("num-abc");
  
  int counter = 0;
  int isLightOn = 0;
  // 按下按键即会执行该函数
  void button1_callback(const String & state) {
      BLINKER_LOG("get button state: ", state);
      isLightOn = !isLightOn;
      digitalWrite(LED_BUILTIN, isLightOn);
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
  
  void setup() {
      // 初始化串口
      Serial.begin(115200);
  
      #if defined(BLINKER_PRINT)
//          BLINKER_DEBUG.stream(BLINKER_PRINT);
          BLINKER_DEBUG.stream(Serial);
          BLINKER_DEBUG.debugAll();
      #endif
      
      // 初始化有LED的IO
      pinMode(LED_BUILTIN, OUTPUT);
      digitalWrite(LED_BUILTIN, LOW);
      // 初始化blinker
      Blinker.begin(auth, ssid, pswd);
      Blinker.attachData(dataRead);
      Button1.attach(button1_callback);
      Button1.text("已关灯");
      Button1.color("DarkRed");
      Button1.print();
  
      BlinkerDuerOS.attachPowerState(duerPowerState);
      BlinkerDuerOS.attachQuery(duerQuery);

      BlinkerMIOT.attachPowerState(miotPowerState);
      BlinkerMIOT.attachQuery(miotQuery);
  }
  
  void loop() {
      Blinker.run();
  }
  void duerQuery(int32_t queryCode)
  {
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
  void duerPowerState(const String & state)
  {
      BLINKER_LOG("need set power state: ", state);
  
      if (state == BLINKER_CMD_ON) {
          digitalWrite(LED_BUILTIN, HIGH);
          BlinkerDuerOS.powerState("on");
          BlinkerDuerOS.print();
          BLINKER_LOG("Light is: ON");
          isLightOn = 1;
      }
      else if (state == BLINKER_CMD_OFF) {
          digitalWrite(LED_BUILTIN, LOW);
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

  void miotQuery(int32_t queryCode)
  {
      BLINKER_LOG("MIOT Query codes: ", queryCode);
  
      switch (queryCode)
      {
          case BLINKER_CMD_QUERY_ALL_NUMBER :
              BLINKER_LOG("MIOT Query All");
              BlinkerMIOT.powerState(isLightOn ? "on" : "off");
              BlinkerMIOT.print();
              break;
          case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
              BLINKER_LOG("MIOT Query Power State");
              BlinkerMIOT.powerState(isLightOn ? "on" : "off");
              BlinkerMIOT.print();
              break;
          default :
              BlinkerMIOT.powerState(isLightOn ? "on" : "off");
              BlinkerMIOT.print();
              break;
      }
  }
  void miotPowerState(const String & state)
  {
      BLINKER_LOG("need set power state: ", state);
  
      if (state == BLINKER_CMD_ON) {
          digitalWrite(LED_BUILTIN, HIGH);
          BlinkerMIOT.powerState("on");
          BlinkerMIOT.print();
          BLINKER_LOG("Light is: ON");
          isLightOn = 1;
      }
      else if (state == BLINKER_CMD_OFF) {
          digitalWrite(LED_BUILTIN, LOW);
          BlinkerMIOT.powerState("off");
          BlinkerMIOT.print();
          BLINKER_LOG("Light is: OFF");
          isLightOn = 0;
      }
  }
