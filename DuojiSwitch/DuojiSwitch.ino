#include <Servo.h>
#include <OneButton.h>

#define SERVO_PIN 2
#define SWITCH_PIN 14
Servo servo1;
OneButton button(SWITCH_PIN, true);
bool isOn = false;
int switchNum = 120;
int toNum = 120;
ICACHE_RAM_ATTR void checkTicks() {
  // include all buttons here to be checked
  button.tick(); // just call tick() to check the state.
}
void singleClick() {
  Serial.println("clicked");
  if (isOn) {
    Serial.println("from 0 to 120");
    switchNum = 0;
    toNum = 120;
  } else {
    Serial.println("from 120 to 0");
    switchNum = 120;
    toNum = 0;
  }
  isOn = !isOn;
} // singleClick
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  servo1.attach(SERVO_PIN);
  
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), checkTicks, CHANGE);
  button.attachClick(singleClick);

  servo1.write(switchNum);
  delay(1000);
}

void loop() {
  if (switchNum != toNum) {
    if (switchNum > toNum) {
      for (int i = switchNum; i >= toNum; i = i - 5) {
        servo1.write(i);
        delay(5);
      }
    } else {
      for (int i = switchNum; i <= toNum; i = i + 5) {
        servo1.write(i);
        delay(5);
      }
    }
  }
  delay(800);
  servo1.write(60);
  switchNum = toNum;
  button.tick();
}
