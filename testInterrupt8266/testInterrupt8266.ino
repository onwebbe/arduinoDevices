void setup() {
  // put your setup code here, to run once:
  attachInterrupt(digitalPinToInterrupt(4), dischargeInterrupt, FALLING);
}
void dischargeInterrupt() {}
void loop() {
  // put your main code here, to run repeatedly:
  delay(10);
}
