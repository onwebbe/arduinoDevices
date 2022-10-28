#define LIGHT_P 4
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LIGHT_P, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LIGHT_P, LOW);
  Serial.println("LOW");
  delay(2000);
  digitalWrite(LIGHT_P, HIGH);
  Serial.println("HIGH");
  delay(2000);
}
