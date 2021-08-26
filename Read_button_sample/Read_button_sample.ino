#define READ_PIN 6
void setup() {
  // put your setup code here, to run once:
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(READ_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int buttonState = digitalRead(READ_PIN);
  Serial.println(buttonState);
  // switch off is 0
  // switch on is 1
  delay(100);
}
