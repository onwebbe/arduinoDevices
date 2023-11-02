void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);

}

void loop() {
//  analogWrite(12, 100); 
//  analogWrite(13, 0);
//  analogWrite(14, 100);
  delay(2000);
  digitalWrite(12,HIGH);
  delay(2000);
  digitalWrite(12,LOW);
  digitalWrite(13,HIGH);
  delay(2000);
  digitalWrite(13,LOW);
  digitalWrite(14,HIGH);
  delay(2000);
  digitalWrite(14,LOW);
  delay(2000);
//  digitalWrite(12,HIGH);
//  digitalWrite(13,HIGH);
//  delay(500);
//  digitalWrite(12,LOW);
//  digitalWrite(13,LOW);
//  delay(500);
//  digitalWrite(13,HIGH);
//  digitalWrite(14,HIGH);
//  delay(500);
//  digitalWrite(13,LOW);
//  digitalWrite(14,LOW);
//  delay(500);
//  digitalWrite(12,HIGH);
//  digitalWrite(14,HIGH);
//  delay(500);
//  digitalWrite(12,LOW);
//  digitalWrite(14,LOW);
//  delay(500);
//  digitalWrite(12,HIGH);
//  digitalWrite(13,HIGH);
//  digitalWrite(14,HIGH);
//  delay(500);
//  digitalWrite(12,LOW);
//  digitalWrite(13,LOW);
//  digitalWrite(14,LOW);
//  delay(500);
  // put your main code here, to run repeatedly:

}
