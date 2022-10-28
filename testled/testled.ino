void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("start");
}

void loop() {
  pinMode(16, OUTPUT);
//  digitalWrite(16, HIGH);
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 200; i = i + 2) {
    analogWrite(16, i);
    delay(100);
  }
  for(int i = 200; i > 0; i = i - 2) {
    analogWrite(16, i);
    delay(10);
  }
//  digitalWrite(16, HIGH);
//  delay(1000);
//  digitalWrite(16, LOW);
//  delay(1000);
  
}
