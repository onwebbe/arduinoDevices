void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("done");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("looping");
  delay(5000);
}
