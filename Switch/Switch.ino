#define SWITCH_PIN 14
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(SWITCH_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("ON");
  digitalWrite(SWITCH_PIN, HIGH);
  delay(1000);
  Serial.println("OFF");
  digitalWrite(SWITCH_PIN, LOW);
  delay(1000);
}
