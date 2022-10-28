#include <Servo.h>
#define SERVO_PIN 2
Servo servo1;
void setup() {
  // put your setup code here, to run once:
  servo1.attach(SERVO_PIN);
  servo1.write(0);
}

void loop() {
  servo1.write(80);
  delay(5000);
  // put your main code here, to run repeatedly:
  for(int i = 80; i <= 160; i++) {
    servo1.write(i);
    delay(30);
  }
  delay(5000);
  for(int i = 160; i >= 0; i--) {
    servo1.write(i);
    delay(30);
  }
  servo1.write(0);
  delay(5000);
}
