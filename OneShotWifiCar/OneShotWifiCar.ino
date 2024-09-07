#include "DriverControl.h"
#include <analogWrite.h>
#include <Adafruit_PCF8574.h>
/* Example for 1 output LED that is connected from power to the GPIO expander pin #7
 * Note the LEDs must be connected with the CATHODES to the expander, to SINK current!
 * The PCF8574 cannot SOURCE current!
 */

Adafruit_PCF8574 pcf8574;
DriverControl *driver;
// PCF8574 ex1(0x20);
void setup() {
  // while (!Serial) {
  //   delay(10);
  // }

  // 初始化调试串口波特率
  Serial.begin(115200);
  pcf8574.begin(0x20, &Wire);
  driver = new DriverControl(&pcf8574, 14, 2, 15, 3, 255, 255);
  // DriverControl driver(14, 2, 15, 3, 255, 255);
  // if (!pcf.begin(0x20, &Wire)) {
  //   Serial.println("Couldn't find PCF8574");
  // }
  // driver.init(14, 2, 15, 3, 255, 255, 255);
  Serial.println("found");
  Serial.println(SDA);
  Serial.println(SCL);
  driver->setLeftAPCF(false);
  driver->setLeftBPCF(true);
  driver->setENLeftPCF(false);
  driver->setRightAPCF(false);
  driver->setRightBPCF(true);
  driver->setENRightPCF(false);
  driver->setup();
  // pcf8574.pinMode(2, OUTPUT);
  Serial.println("----------Start----------");
  
  // if (!pcf.begin(0x20, &Wire)) {
  //   Serial.println("Couldn't find PCF8574");
  //   while (1);
  // }
  // Serial.println("--------------------");
  // pcf.pinMode(7, OUTPUT);
  
  // pinMode(14, OUTPUT);
  // pinMode(15, OUTPUT);
  // pcf.pinMode(2, OUTPUT);
  // pcf.pinMode(3, OUTPUT);
}

void loop() {
  // pcf8574->digitalWrite(2, HIGH);
  // pcf.digitalWrite(0, HIGH);
  // Serial.print("ON");
  // delay(5000);
  moveForward();
  // delay(5000);
  
  // delay(5000);
  driver->goStaight(255);
  // pcf8574.digitalWrite(2, HIGH);
  delay(5000);
  moveBackward();
  driver->goBack(255);
  // pcf8574.digitalWrite(2, LOW);
  delay(5000);
  // driver->turnLeft(255);
  // delay(5000);
  // driver->turnRight(255);
  // delay(5000);
  // pcf.digitalWrite(7, LOW);  // turn LED on by sinking current to ground
  // delay(100);
  // pcf.digitalWrite(7, HIGH); // turn LED off by turning off sinking transistor
  // delay(100);
}


void moveForward() {
  digitalWrite(15, HIGH);
  pcf8574.digitalWrite(3, LOW);
}
void moveBackward() {
  digitalWrite(15, LOW);
  pcf8574.pinMode(3, HIGH);
}
void turnLeft() {

}
void turnRight() {

}