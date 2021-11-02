int pin = 16; //定义引脚为D2
unsigned long duration=0; //定义duration变量为无符号长整数型变量
int i = 0;
void setup()
{
  Serial.begin(9600); //串口波特率为9600
  pinMode(pin, INPUT); //设置引脚为输入模式
  delay(5000);
  Serial.println("begin");
}


void loop()
{
  duration += pulseIn(pin, LOW, 10000);
  Serial.println(duration);
  if (duration > 0) {
    Serial.println("calculated");
    Serial.println(duration);
    
    double twoDuration = duration * 2;
    double rpm = 60000 / twoDuration;
    Serial.println("rpm:" + String(rpm));
  }
}
