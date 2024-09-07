#include <Wire.h>
#define RESET_PIN 2
/**

* SCL <--> D1

* SDA <--> D2

*/

void setup() {

  // 初始化I2C总线

  Wire.begin();

  // 初始化调试串口波特率

  Serial.begin(115200);
  Serial.println(SDA);
  Serial.println(SCL);
  
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);

}

 


void loop() {

  byte error, address;

  int nDevices;

  // 串口输出调试信息

  Serial.println("Scanning I2C Devices....");

  nDevices = 0;

  for (address = 1; address < 127; address++ ) {

    // The i2c_scanner uses the return value of

    // the Write.endTransmisstion to see if

    // a device did acknowledge to the address.

    // 发送1次从机地址

    Wire.beginTransmission(address);

    // 等待从机响应

    error = Wire.endTransmission();

    if (error == 0) {

      Serial.print("I2C device found at address 0x");

      // 显示当前查询的从机地址

      if (address < 16)

      {

        Serial.print("0");

      }

      Serial.print(address, HEX);

      Serial.println(" !");

      // 找到的从机数量自增

      nDevices++;

    } else if (error == 4) {

      Serial.print("Unknow error at address 0x");

      if (address < 16)

        Serial.print("0");

      Serial.println(address, HEX);

    }

  }

  // 所有从机地址遍历完毕

  if (nDevices == 0)

    Serial.println("No I2C devices found\n");

  else

    Serial.println("done\n");

  delay(5000); // wait 5 seconds for next scan

}
