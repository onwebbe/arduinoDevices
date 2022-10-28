#include <Servo.h>
#define SERVO_PIN 4
Servo servo1;
void setup() {
  // put your setup code here, to run once:
  servo1.attach(SERVO_PIN);
}

void loop() {
  // put your main code here, to run repeatedly:
  servo1.write(20);
}
