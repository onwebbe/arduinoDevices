#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define BUILTIN_LED 2
char P_NAME[] = "pankey_asus";           //设置热点名称
char P_PSWD[] = "1234554321";          //设置热点密码
char sub[] = "Sub/100052";    //设置设备SUB名称
char pub[] = "Pub/100052";    //设置设备PUB名称
 
const char *ssid = P_NAME;
const char *password = P_PSWD;
const char *mqtt_server = "121.5.58.100";
String reStr;
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
 
void setup_wifi()
{
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("not connect");
    delay(500);
  }
  Serial.println("connected");
  randomSeed(micros());
}
 
void setup()
{
  Serial.begin(115200);
  setup_wifi();
  
}
 
void loop()
{
  long rssi = WiFi.RSSI();
  Serial.print("RSSI:");
  Serial.println(rssi);
  delay(500);
}