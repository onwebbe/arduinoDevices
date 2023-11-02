/*
 * MotorKnob
 *
 * A stepper motor follows the turns of a potentiometer
 * (or other sensor) on analog input 0.
 *
 * http://www.arduino.cc/en/Reference/Stepper
 * This example code is in the public domain.
 */

#include <Stepper.h>

// change this to the number of steps on your motor
#define STEPS 100

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 12, 13, 14, 15);

// the previous reading from the analog input
int previous = 0;

void setup() {
  Serial.begin(15200);
  // set the speed of the motor to 30 RPMs
  stepper.setSpeed(30);
}

void loop() {
  // get the sensor value
  int val = analogRead(0);
  Serial.println(val);
  // move a number of steps equal to the change in the
  // sensor reading
  for(int i = 0; i < 100; i ++) {
    stepper.step(i);
  }
  

  // remember the previous value of the sensor
  previous = val;
}
