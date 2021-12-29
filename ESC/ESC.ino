/*
        Arduino Brushless Motor Control
     by Dejan, https://howtomechatronics.com
*/
#include <Servo.h>
#include "ESC.h"
ESC8266 esc1(4);
void setup() {
  Serial.begin(9600);
  esc1.init();
}
int start = 300;
void loop() {
  esc1.increase();
  Serial.println(esc1._currentSpeed);
  if (esc1.currentSpeed == esc1.maxSpeed) {
    esc1.changeSpeed(esc1.minSpeed);
  }
  delay(2000);
}
