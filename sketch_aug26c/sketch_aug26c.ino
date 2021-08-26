#define AnalogRead_PIN 5
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(AnalogRead_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(AnalogRead_PIN);
  Serial.println(val);
  delay(100);
}
