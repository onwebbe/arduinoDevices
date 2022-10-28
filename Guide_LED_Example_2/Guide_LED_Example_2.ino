#define LED_PIN 4
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 256; i = i + 10) {
    analogWrite(LED_PIN, i);
    delay(50);
  }
  for(int i = 255; i>= 0 ;i = i - 10) {
    analogWrite(LED_PIN, i);
    delay(50);
  }
}
