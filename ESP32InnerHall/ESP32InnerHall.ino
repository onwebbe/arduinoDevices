void setup() {
  Serial.begin(9600);
}
 
void loop() {
 
    int measurement = 0;
    measurement = hallRead();
    Serial.print("Hall sensor measurement: ");
    Serial.println(measurement); 
 
    delay(1000);
}
