#define echoPin 4
#define trigPin 5
#include "SRF05.h"
SRF05 srf(echoPin, trigPin);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  float distance = srf.getDistance();
  Serial.println("distance " + String(distance) + "cm");
  delay(500);
}
