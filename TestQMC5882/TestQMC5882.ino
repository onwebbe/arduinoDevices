#include <Wire.h>
uint8_t address = 0x0D;
int maxx=-15535, minx=15535, maxy=-15535, miny=15535;
int x,y,z;
long lastMin = millis();
void setup() {
  Wire.begin();
  delay(100);
  Serial.begin(115200);
  Serial.println("0");
  Wire.beginTransmission(address);
  Serial.println(Wire.endTransmission());
  Serial.println("1");
  writeReg(0x0B, 0x01); //手册上推荐一开始在0B位置上写0x01
  Serial.println("2");
  writeReg(0x09, 0b00000001|0b00001100|0b00010000|0b00000000); //Mode_Continuous,ODR_200Hz,RNG_8G,OSR_512, 设置模式等配置
  lastMin = millis();
}

void loop() {
  if ((millis() - lastMin) > 10000) {
    Serial.print("maxX: ");
    Serial.println(maxx);
    Serial.print("maxY: ");
    Serial.println(maxy);
    Serial.print("minX: ");
    Serial.println(minx);
    Serial.print("minY: ");
    Serial.println(miny);
    double outX, outY;
    readXYZ(&x, &y, &z);
    calculateOffset3(minx, maxx, miny, maxy, x, y, &outX, &outY);
    Serial.print("Calculated OutX:");
    Serial.println(outX);
    Serial.print("Calculated OutY:");
    Serial.println(outY);
    // https://blog.csdn.net/weixin_50622833/article/details/120188131 - 还没有使用的offset设置方法
    delay(5000);
  } else {
    // Serial.println("3");
    // Serial.println(readChipInfo());
    // Serial.println("stop");
    
    Serial.println(readXYZ(&x, &y, &z));
    Serial.print("X: ");
    Serial.println(x);
    Serial.print("Y: ");
    Serial.println(y);
    Serial.print("Z: ");
    Serial.println(z);
    if (x > maxx) {
      maxx = x;
    }
    if (x < minx) {
      minx = x;
    }
    if (y > maxy) {
      maxy = y;
    }
    if (y < miny) {
      miny = y;
    }
    // float azimuth = atan2((float)x, (float)y) * 180.0/PI;
    // Serial.println(azimuth < 0?360 + azimuth:azimuth);

    Serial.print("maxX: ");
    Serial.println(maxx);
    Serial.print("maxY: ");
    Serial.println(maxy);
    Serial.print("minX: ");
    Serial.println(minx);
    Serial.print("minY: ");
    Serial.println(miny);
    delay(200);
  }
}
void writeReg(byte Reg,byte val){
  Wire.beginTransmission(address); //start talking
  Wire.write(Reg); // Tell the HMC5883 to Continuously Measure
  Wire.write(val); // Set the Register
  Serial.println("end write");
  Serial.print(Wire.endTransmission());
  Serial.println(" from end write");
}

void calculateOffset1(int minX, int maxX, int minY, int maxY, int x, int y, double* outX, double* outY) {
  // https://blog.sina.com.cn/s/blog_402c071e0102v8ie.html
  double xs = 1;
  double ys = (maxX - minX) / (maxY - minY);
  double xb = xs * (1 / (maxX - minX) - maxX);
  double yb = ys * (1 / (maxY - minY) - maxY);
  *outX = x * xs + xb;
  *outY = y * ys + yb;
}

void calculateOffset2(int minX, int maxX, int minY, int maxY, int x, int y, double* outX, double* outY) {
  // https://blog.sina.com.cn/s/blog_402c071e0102v8ie.html
  double offsetX = (maxX + minX) / 2;
  double offsetY = (maxY + minY) / 2;
  double kx = 2 / (maxX - minX);
  double ky = 2 / (maxY - minY);
  *outX = (x - offsetX) * kx;
  *outY = (y - offsetY) * ky;
}

void calculateOffset3(int minX, int maxX, int minY, int maxY, int x, int y, double* outX, double* outY) {
  // https://blog.sina.com.cn/s/blog_402c071e0102v8ie.html
  double offsetX = (maxX + minX) / 2;
  double offsetY = (maxY + minY) / 2;
  *outX = (x - offsetX);
  *outY = (y - offsetY);
}
/**
 * read values from device
 * @return status value:
 *  - 0:success
 *  - 1:data too long to fit in transmit buffer
 *  - 2:received NACK on transmit of address
 *  - 3:received NACK on transmit of data
 *  - 4:other error
 *  - 8:overflow (magnetic field too strong)
 */
int readXYZ(int* x,int* y,int* z){
  Wire.beginTransmission(address);
  Wire.write(0x00);
  int err = Wire.endTransmission();
  if (err) {return err;}
  Wire.requestFrom(address, (uint8_t)7);
  *x = (int)(int16_t)(Wire.read() | Wire.read() << 8);
  *y = (int)(int16_t)(Wire.read() | Wire.read() << 8);
  *z = (int)(int16_t)(Wire.read() | Wire.read() << 8);
  byte overflow = Wire.read() & 0x02;
  return overflow << 2;
}

// int readChipInfo(){
//   Wire.beginTransmission(address);
//   Wire.write(0x0D);
//   int err = Wire.endTransmission();
//   if (err) {return err;}
//   Wire.requestFrom(address, (uint8_t)2);
//   return (int)(int16_t)(Wire.read() | Wire.read() << 8);
// }

int readChipInfo(){
  Wire.beginTransmission(address);
  Wire.write(0x0D);
  int err = Wire.endTransmission();
  if (err) {
    Serial.println("error");
    return err;
  }
  Wire.requestFrom(address, (uint8_t)1);
  return (int)(int16_t)(Wire.read());
}