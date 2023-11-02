int _correctionFactor = 1;
void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(12, OUTPUT);
  pinMode(13, INPUT);
  digitalWrite(12, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(12, HIGH);
  delayMicroseconds(10);
  digitalWrite(12, LOW);
  uint32_t duration = pulseIn(13, HIGH, 300000);
  if (_correctionFactor != 1)
  {
     duration = round(duration * _correctionFactor);
  }
  Serial.println(round(duration * 340 / 200));
}
