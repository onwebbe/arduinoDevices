#define RESET_NETWORK_BUTTON_PIN 4


void setup() {
  Serial.begin(112500);
  // put your setup code here, to run once:
  pinMode(RESET_NETWORK_BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(RESET_NETWORK_BUTTON_PIN), checkTicks, CHANGE);
}
void singleClick() {
  Serial.println("Button Clicked");
}
void checkTicks() {
  // include all buttons here to be checked
  Serial.println("Button Clicked");
}
void loop() {
  // put your main code here, to run repeatedly:
  delay(10);
}
