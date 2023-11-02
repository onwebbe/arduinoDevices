int startTime = -1;
int endTime = -1;
bool isHigh = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(16, INPUT);
}
void loop() {
  uint8_t val = digitalRead(16);
  // put your main code here, to run repeatedly:
  if (val == HIGH && isHigh == false) {
    isHigh = true;
    startTime = millis();
    endTime = -1;
  } else if (val == LOW && isHigh == true) {
    isHigh = false;
    endTime = millis();
  } else if (val == HIGH && isHigh == true && (millis() - startTime) >= 3000) {
    Serial.println(millis() - startTime);
    startTime = -1;
    endTime = -1;
    isHigh = false;
  }
  if (startTime > 0 && endTime > 0) {
    Serial.println(endTime - startTime);
    startTime = -1;
    endTime = -1;
  }
}
