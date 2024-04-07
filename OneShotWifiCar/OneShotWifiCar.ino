#include <Adafruit_PCF8574.h>

/* Example for 1 output LED that is connected from power to the GPIO expander pin #7
 * Note the LEDs must be connected with the CATHODES to the expander, to SINK current!
 * The PCF8574 cannot SOURCE current!
 */

Adafruit_PCF8574 pcf;
void setup() {
  while (!Serial) { delay(10); }
  Serial.begin(115200);

  if (!pcf.begin(0x20, &Wire)) {
    Serial.println("Couldn't find PCF8574");
    while (1);
  }
  pcf.pinMode(7, OUTPUT);
  
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pcf.pinMode(2, OUTPUT);
  pcf.pinMode(3, OUTPUT);
}

void loop() {
  pcf.digitalWrite(7, LOW);  // turn LED on by sinking current to ground
  delay(100);
  pcf.digitalWrite(7, HIGH); // turn LED off by turning off sinking transistor
  delay(100);
}


void moveForward() {
  digitalWrite(14, HIGH);
  pcf.digitalWrite(2, HIGH);
}
void moveBackward() {
  digitalWrite(14, LOW);
  pcf.pinMode(2, LOW);
}
void turnLeft() {

}
void turnRight() {

}