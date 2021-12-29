
struct ControllerData {
 byte leftController_UP_DOWN;
 byte leftController_LEFT_RIGHT;
 byte rightController_UP_DOWN;
 byte rightController_LEFT_RIGHT;
 byte switchA;
 byte switchB;
};
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  ControllerData controllerData;
  controllerData.leftController_UP_DOWN = 10;
  controllerData.leftController_LEFT_RIGHT = 11;
  controllerData.rightController_UP_DOWN = 12;
  controllerData.rightController_LEFT_RIGHT = 13;
  controllerData.switchA = 14;
  controllerData.switchB = 15;
  byte* sourceByte = convertStructToByte(controllerData);

  ControllerData newCt = convertByteToStruct(sourceByte);

  Serial.println(newCt.leftController_UP_DOWN);
  Serial.println(newCt.switchA);
}
byte* I2C_writeAnything (const ControllerData& value) {
  const byte * p = (const byte*) &value;
  byte* data = new byte[sizeof(value)];
  unsigned int i;
  for (i = 0; i < sizeof value; i++) {
    data[i] = *p++;
  }
  return data;
}
byte* convertStructToByte(ControllerData struct_data) {
  byte b[sizeof(struct_data)];
  memcpy(b, &struct_data, sizeof(struct_data));
  return b;
}
ControllerData convertByteToStruct(byte* byte_data) {
  ControllerData struct_data;
  memcpy(&struct_data, byte_data, sizeof(struct_data));
}
void loop() {
  // put your main code here, to run repeatedly:

}
