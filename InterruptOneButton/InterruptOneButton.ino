/*
 InterruptOneButton.ino - Example for the OneButtonLibrary library.
 This is a sample sketch to show how to use the OneButtonLibrary
 to detect double-click events on a button by using interrupts. 
 The library internals are explained at
 http://www.mathertel.de/Arduino/OneButtonLibrary.aspx

 Good article on Arduino UNO interrupts:
 https://arduino.stackexchange.com/questions/30968/how-do-interrupts-work-on-the-arduino-uno-and-similar-boards
 ... and the processor datasheet.
  
 Setup a test circuit:
 * Connect a pushbutton to the PIN_INPUT (see defines for processor specific examples) and ground.
 * The PIN_LED (see defines for processor specific examples) is used for output attach a led and resistor to VCC.
   or maybe use a built-in led on the standard arduino board.
   
 The sketch shows how to setup the library and bind the functions (singleClick, doubleClick) to the events.
 In the loop function the button.tick function must be called as often as you like.
 By using interrupts the internal state advances even when for longer time the button.tick is not called.
*/

// 03.03.2011 created by Matthias Hertel
// 01.12.2011 extension changed to work with the Arduino 1.0 environment
// 04.11.2017 Interrupt version created.
// 04.11.2017 Interrupt version using attachInterrupt.

#include "OneButton.h"

#define PIN_INPUT 4
#define PIN_LED 2

// Setup a new OneButton on pin PIN_INPUT
// The 2. parameter activeLOW is true, because external wiring sets the button to LOW when pressed.
OneButton button(PIN_INPUT, true);

// current LED state, staring with LOW (0)
int ledState = LOW;

// save the millis when a press has started.
unsigned long pressStartTime;

// In case the momentary button puts the input to HIGH when pressed:
// The 2. parameter activeLOW is false when the external wiring sets the button to HIGH when pressed.
// The 3. parameter can be used to disable the PullUp .
// OneButton button(PIN_INPUT, false, false);


// This function is called from the interrupt when the signal on the PIN_INPUT has changed.
// do not use Serial in here.

ICACHE_RAM_ATTR void checkTicks() {
  // include all buttons here to be checked
  button.tick(); // just call tick() to check the state.
}
// this function will be called when the button was pressed 1 time only.
void singleClick() {
  Serial.println("singleClick() detected.");
} // singleClick

// setup code here, to run once:
void setup() {
  Serial.begin(115200);
  Serial.println("One Button Example with interrupts.");

  // enable the led output.
  pinMode(PIN_LED, OUTPUT); // sets the digital pin as output
  digitalWrite(PIN_LED, ledState);

  // setup interrupt routine
  // when not registering to the interrupt the sketch also works when the tick is called frequently.
  attachInterrupt(digitalPinToInterrupt(PIN_INPUT), checkTicks, CHANGE);

  // link the xxxclick functions to be called on xxxclick event.
  button.attachClick(singleClick);

  // A1-Option for UNO:
  // it is possible to use e.g. A1 but then some direct register modifications and an ISR has to be used:
  // You may have to modify the next 2 lines if using another pin than A1
  // PCICR |= (1 << PCIE1);   // This enables Pin Change Interrupt 1 that covers the Analog input pins or Port C.
  // PCMSK1 |= (1 << PCINT9); // This enables the interrupt for pin 1 of Port C: This is A1.

} // setup

// A1-Option for UNO:
// The Interrupt Service Routine for Pin Change Interrupt 1
// This routine will only be called on any signal change on A1: exactly where we need to check.
// ISR(PCINT1_vect)
// {
//   // keep watching the push button:
//   button.tick(); // just call tick() to check the state.
// }


// main code here, to run repeatedly:
void loop() {
  // keep watching the push button, even when no interrupt happens:
  button.tick();

  // You can implement other code in here or just wait a while
  delay(10);
} // loop


// End
