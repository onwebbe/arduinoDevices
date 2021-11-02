int pinInterrupt = 16; //接中断信号的脚  
int count = 0;
void onChange()  
{   
  Serial.println("changed to low");
}  
  
void setup()  
{  
   Serial.begin(9600); //打开串口  
  
   pinMode( pinInterrupt, INPUT);//设置管脚为输入  
     
   //Enable中断管脚, 中断服务程序为onChange(), 监视引脚变化  
   attachInterrupt( digitalPinToInterrupt(pinInterrupt), onChange, FALLING);  
}  
  
void loop()  
{  
   count = 0;
//   delay(10000);
//   Serial.println(count); 
//   delay(1000);
} 
