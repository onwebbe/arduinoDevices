/*
 * PCF8575 GPIO Port Expand
 * http://nopnop2002.webcrow.jp/WeMos/WeMos-25.html
 *
 * PCF8575    ----- WeMos
 * A0         ----- GRD
 * A1         ----- GRD
 * A2         ----- GRD
 * VSS        ----- GRD
 * VDD        ----- 5V/3.3V
 * SDA        ----- GPIO_4(PullUp)
 * SCL        ----- GPIO_5(PullUp)
 *
 * P0     ----------------- LED0
 * P1     ----------------- LED1
 * P2     ----------------- LED2
 * P3     ----------------- LED3
 * P4     ----------------- LED4
 * P5     ----------------- LED5
 * P6     ----------------- LED6
 * P7     ----------------- LED7
 * P8     ----------------- LED8
 * P9     ----------------- LED9
 * P10     ----------------- LED10
 * P11     ----------------- LED11
 * P12     ----------------- LED12
 * P13     ----------------- LED13
 * P14     ----------------- LED14
 * P15     ----------------- LED15
 *
 */

#include "Arduino.h"
#include "PCF8575.h"  // https://github.com/xreef/PCF8575_library
int servopin=4;//定义数字接口9 连接伺服舵机信号线
int myangle;//定义角度变量
int pulsewidth;//定义脉宽变量
int val;
// Set i2c address
PCF8575 pcf8575(0x20);


void servopulse(int servopin,int myangle) {//定义一个脉冲函数
  pulsewidth=(myangle*11)+500;//将角度转化为500-2480 的脉宽值
  pcf8575.digitalWrite(0, HIGH);
  delayMicroseconds(pulsewidth);//延时脉宽值的微秒数
  pcf8575.digitalWrite(0, LOW);
  delay(20-pulsewidth/1000);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("start");
  pcf8575.pinMode(0, OUTPUT);
  pcf8575.begin();
  pcf8575.pinMode(0, OUTPUT);
  Serial.println("pcf8575 begin");
}

void loop()
{
  for(val = 0;val < 180; val = val + 10) {
      for(int i=0;i<=50;i++) { //给予舵机足够的时间让它转到指定角度
        servopulse(servopin,val);//引用脉冲函数
      }

      delay(1000);
  }
}
