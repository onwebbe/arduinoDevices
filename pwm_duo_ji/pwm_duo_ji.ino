#include <Servo.h>
Servo servo;
int pos = 0;
void setup() {
  // put your setup code here, to run once:
  servo.attach(3);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(pos = 0; pos < 180; pos+=10) {
    servo.write(pos);
    delay(1000);
  }
}
