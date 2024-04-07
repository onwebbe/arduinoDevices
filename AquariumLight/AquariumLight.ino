#include "HAMqttWifiManager.h"
#include "HAMqttDeviceBase.h"
#include "HAMqttLight.h"
#include "HAMqttSwitch.h"
#include "HAMqttSensor.h"
#include "HAMqttServoGoAndBack.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

// #define DEVICE_NAME "AquariumLight-30551c38"
// #define DEVICE_NAME "AquariumLight-d32016a7"
#define DEVICE_NAME "AquariumLight-servo"  // test


// #define DEVICE_NAME "AquariumLight-test"
// #define CLIENT_ID "AquariumLight-2"
#define CLIENT_ID "AquariumLight-4"
#define CLIENT_PASSWORD "AquariumLight_123456"

#define LIGHT_PIN 13
#define HEATER_PIN 12
#define PUMB_PIN 14
#define RESET_NETWORK_BUTTON_PIN 16
#define SWITCH_BUTTON_PIN 5
#define SERVO_PIN 4


String YUGANG_STATUS_CHANNEL;
const int TEMPERATURE_PIN = A0;

HAMqttWifiManager wm;
WiFiClient espClient;
PubSubClient* client = new PubSubClient(espClient);
HAMqttSwitch heater(client, HEATER_PIN, "heater", DEVICE_NAME, "homedevice/log");
HAMqttServoGoAndBack feeder(client, SERVO_PIN, "feeder", DEVICE_NAME, "homedevice/log");
HAMqttSensor aqTemperature(client, "aquariumTemperature", DEVICE_NAME, "homedevice/log");
HAMqttSensorConfig* temperatureConfig = new HAMqttSensorConfig("°C", "temperature");

HAMqttLight aqLight(client, LIGHT_PIN, "light", DEVICE_NAME, "homedevice/log");
HAMqttLight aqPumb(client, PUMB_PIN, "pumb", DEVICE_NAME, "homedevice/log");

double Rs = 200000;//R1取值为150k
double Vcc = 3.3;
double temperatureValue[242];
double rValue[242];

int temperatureGap = 5000;
int temperatureGapTest = temperatureGap / 10;
long lastTime;
long currentTime;

long lastTemperatureTime;
long currentTemperatureTime;
int currentTemperatureCount = 0;
int temperatureTemp[100];

void setup() {
  Serial.begin(115200);
  Serial.println("start");
  for(int i = 0; i < 100; i++) {
    temperatureTemp[i] = 0;
  }

  temperatureConfig->setType(2);
  YUGANG_STATUS_CHANNEL = "homedevice/sensor/" + String(DEVICE_NAME) + ".aquariumStatus/text/status";
  Serial.println("AquariumLight Status Channel:");
  Serial.println(YUGANG_STATUS_CHANNEL);
  aqTemperature.addSensorConfig(temperatureConfig);
  initTemperature();
  // put your setup code here, to run once:
  aqPumb.setFullPowerBeforeLightChange(1500);
  heater.setup();
  aqPumb.setup();
  aqLight.setup();
  feeder.setup();
  feeder.setDelayTime(400);
  feeder.setStartAngle(0);
  feeder.setEndAngle(180);
  aqTemperature.setup();

  wm.setDeviceName(DEVICE_NAME);
  wm.setClientId(CLIENT_ID);

  wm.setClientPassword(CLIENT_PASSWORD);
  wm.setClientIP(String("192.168.50.109"));
  wm.setClientPort("9013");
  wm.setNotConnectedCallback(wmNotConnectCallBack);
  wm.setup();
  String mqttId = wm.getClientID();
  String mqttPassword = wm.getClientPassword();
  String mqttServerIP = wm.getClientIP();
  String mqttServerPort = wm.getClientPort();
  Serial.printf("mqttServerIP:%s port:%d", mqttServerIP.c_str(), mqttServerPort.toInt());
  client->setServer(mqttServerIP.c_str(), mqttServerPort.toInt());
  client->setCallback(callback);
  reconnectMqtt();
  wm.saveConfigToFile();
  lastTime = millis();
  lastTemperatureTime = millis();
}
void reconnectMqtt() {
  int i = 0;
  String mqttId = wm.getClientID();
  String mqttPassword = wm.getClientPassword();
  String mqttServerIP = wm.getClientIP();
  String mqttServerPort = wm.getClientPort();
  Serial.print("reconnecting with: ");
  Serial.print(mqttServerIP);
  Serial.print(":::::");
  Serial.print(mqttServerPort);
  Serial.print(":::::");
  Serial.print(mqttId);
  Serial.print(":::::");
  Serial.print(mqttPassword);
  while(!client->connected()) {
    client->setServer(mqttServerIP.c_str(), mqttServerPort.toInt());
    client->setCallback(callback);
    if (client->connect(mqttId.c_str(), "", mqttPassword.c_str())) {
      Serial.println("Connected to MQTT");
      break;
    }
    Serial.println("Connect to mqtt server failed, retrying");
    delay(12000);
    i++;
    if (i == 5) {
      break;
    }
  }
  if (i >= 5) {
    delay(5000);
    ESP.restart();
  }
  // String channel = String("homedevice/#/") + String(DEVICE_NAME) + String("/+/status");
  // client->subscribe(channel.c_str());

  heater.doSubscribe();
  heater.publishDiscover();
  heater.publishSwitchStatus();


  aqLight.doSubscribe();
  aqLight.publishDiscover();
  aqLight.publishSwitchStatus();
  aqLight.publishBrightStatus();

  aqTemperature.doSubscribe();
  aqTemperature.publishDiscover();
  aqTemperature.publishData();

  aqPumb.doSubscribe();
  aqPumb.publishDiscover();
  aqPumb.publishSwitchStatus();
  aqPumb.publishBrightStatus();

  feeder.doSubscribe();
  feeder.publishDiscover();
  feeder.publishServoGoBackStatus();

  
}
void callback(char* topic, byte* payload, unsigned int length) {
  String topicString(topic);
  char payloadChar[length];
  for(int i = 0; i < length; i++) {
    payloadChar[i] = (char) payload[i];
  }
  String payloadString(payloadChar);
  Serial.print("Payload length:");
  Serial.println(length);
  Serial.print("Payload data:");
  Serial.println(payloadChar);
  Serial.print("Message arrived [");
  Serial.print(topicString);
  Serial.print("] [");
  Serial.print(payloadString);
  Serial.println("]");
  heater.callback(topicString, payloadString);
  aqTemperature.callback(topicString, payloadString);
  aqLight.callback(topicString, payloadString);
  aqPumb.callback(topicString, payloadString);
  feeder.callback(topicString, payloadString);
}
void wmNotConnectCallBack() {
  Serial.println("loop-wm-noconnect-Set pin to 0 ");
  heater.beforeReconnect();
  aqTemperature.beforeReconnect();
  aqLight.beforeReconnect();
  aqPumb.beforeReconnect();
  feeder.beforeReconnect();
}
void loop() {
  if (!wm.isNetworkConnected() || !client->connected()) {
    Serial.println("loop-noconnect-network-Set pin to 0 ");
    heater.beforeReconnect();
    aqTemperature.beforeReconnect();
    aqLight.beforeReconnect();
    aqPumb.beforeReconnect();
    feeder.beforeReconnect();
    if (!wm.isNetworkConnected()) {
      wm.reconnect(-1);
    }
    reconnectMqtt();
    heater.afterRconnected();
    aqTemperature.afterRconnected();
    aqLight.afterRconnected();
    aqPumb.afterRconnected();
    feeder.afterRconnected();
  }
  client->loop();
  heater.loop();
  aqLight.loop();
  aqPumb.loop();
  feeder.loop();
  currentTime = millis();
  currentTemperatureTime = millis();
  if (currentTime < lastTime) {
    // Serial.print("-------Temperature Final1:");
    // Serial.println(currentTemperatureCount);
    int totalTempData = 0;
    for(int i = 0; i < 100; i++) {
      totalTempData += temperatureTemp[i];
    }
    int tempPin = totalTempData / currentTemperatureCount;
    // int tempPin = analogRead(TEMPERATURE_PIN);
    temperatureConfig->setData(String(getTemperature(tempPin)));
    aqTemperature.loop();
    client->publish(YUGANG_STATUS_CHANNEL.c_str(), consolidateDeviceStatus().c_str());
    lastTime = millis();
    lastTemperatureTime = millis();
    currentTemperatureCount = 0;
    for(int i = 0; i < 100; i++) {
      temperatureTemp[i] = 0;
    }
  } else if ((currentTime - lastTime) > temperatureGap) {
    // Serial.print("-------Temperature Final2:");
    // Serial.println(currentTemperatureCount);
    int totalTempData = 0;
    for(int i = 0; i < 100; i++) {
      totalTempData += temperatureTemp[i];
    }
    int tempPin = totalTempData / currentTemperatureCount;
    // int tempPin = analogRead(TEMPERATURE_PIN);
    temperatureConfig->setData(String(getTemperature(tempPin)));
    aqTemperature.loop();
    client->publish(YUGANG_STATUS_CHANNEL.c_str(), consolidateDeviceStatus().c_str());
    lastTime = millis();
    lastTemperatureTime = millis();
    currentTemperatureCount = 0;
    for(int i = 0; i < 100; i++) {
      temperatureTemp[i] = 0;
    }
  } 
  else if ((currentTemperatureTime < lastTemperatureTime) || 
              (currentTemperatureTime - lastTemperatureTime) > temperatureGapTest) {
    int pinData = analogRead(TEMPERATURE_PIN);
    temperatureTemp[currentTemperatureCount++] = pinData;
    if (currentTemperatureCount >= 100) {
      currentTemperatureCount = 0;
    }
    // Serial.print("Temperature Temp:");
    // Serial.print(currentTemperatureCount);
    // Serial.print("---------");
    // Serial.print(pinData);
    // Serial.print("---------");
    // Serial.println(getTemperature(pinData));
    
    // Serial.print("---------");
    // Serial.print(currentTemperatureTime);
    // Serial.print(":");
    // Serial.print(lastTemperatureTime);
    // Serial.print(":");
    // Serial.print(currentTemperatureTime - lastTemperatureTime);
    // Serial.print(":");
    // Serial.print(currentTemperatureTime <= lastTemperatureTime);
    // Serial.print(":");
    // Serial.println((currentTemperatureTime - lastTemperatureTime) > temperatureGapTest);
    
    lastTemperatureTime = millis();
  }
}
double calculateTemperature() {
  double totalTem = 0;
  int i = 0;
  for(i = 0; i < sizeof(temperatureTemp) && temperatureTemp[i] > 0; i++) {
    totalTem += temperatureTemp[i];
  }
  return  totalTem / i;
}
String consolidateDeviceStatus() {
  String lightSwitch = aqLight.isSwitchOn()?"ON":"OFF";
  int lightBright = aqLight.getBrightness();
  String pumbSwitch = aqPumb.isSwitchOn()?"ON":"OFF";
  int pumbBright = aqPumb.getBrightness();
  String heaterSwitch = heater.isSwitchOn()?"ON":"OFF";
  String temperature = temperatureConfig->getData();
  String feederSwitch = feeder.isSwitchOn()?"ON":"OFF";
  JsonDocument statusData;
  // Add values in the document
  statusData["lightswitch"] = lightSwitch;
  statusData["lightBright"] = lightBright;
  statusData["pumbswitch"] = pumbSwitch;
  statusData["pumbBright"] = pumbBright;
  statusData["heaterSwitch"] = heaterSwitch;
  statusData["temperature"] = temperature;
  statusData["feederSwitch"] = feederSwitch;
  
  return statusData.as<String>();
}
void initTemperature() {
  temperatureValue[0]=-20;
  temperatureValue[1]=-19.5;
  temperatureValue[2]=-19;
  temperatureValue[3]=-18.5;
  temperatureValue[4]=-18;
  temperatureValue[5]=-17.5;
  temperatureValue[6]=-17;
  temperatureValue[7]=-16.5;
  temperatureValue[8]=-16;
  temperatureValue[9]=-15.5;
  temperatureValue[10]=-15;
  temperatureValue[11]=-14.5;
  temperatureValue[12]=-14;
  temperatureValue[13]=-13.5;
  temperatureValue[14]=-13;
  temperatureValue[15]=-12.5;
  temperatureValue[16]=-12;
  temperatureValue[17]=-11.5;
  temperatureValue[18]=-11;
  temperatureValue[19]=-10.5;
  temperatureValue[20]=-10;
  temperatureValue[21]=-9.5;
  temperatureValue[22]=-9;
  temperatureValue[23]=-8.5;
  temperatureValue[24]=-8;
  temperatureValue[25]=-7.5;
  temperatureValue[26]=-7;
  temperatureValue[27]=-6.5;
  temperatureValue[28]=-6;
  temperatureValue[29]=-5.5;
  temperatureValue[30]=-5;
  temperatureValue[31]=-4.5;
  temperatureValue[32]=-4;
  temperatureValue[33]=-3.5;
  temperatureValue[34]=-3;
  temperatureValue[35]=-2.5;
  temperatureValue[36]=-2;
  temperatureValue[37]=-1.5;
  temperatureValue[38]=-1;
  temperatureValue[39]=-0.5;
  temperatureValue[40]=0;
  temperatureValue[41]=0.5;
  temperatureValue[42]=1;
  temperatureValue[43]=1.5;
  temperatureValue[44]=2;
  temperatureValue[45]=2.5;
  temperatureValue[46]=3;
  temperatureValue[47]=3.5;
  temperatureValue[48]=4;
  temperatureValue[49]=4.5;
  temperatureValue[50]=5;
  temperatureValue[51]=5.5;
  temperatureValue[52]=6;
  temperatureValue[53]=6.5;
  temperatureValue[54]=7;
  temperatureValue[55]=7.5;
  temperatureValue[56]=8;
  temperatureValue[57]=8.5;
  temperatureValue[58]=9;
  temperatureValue[59]=9.5;
  temperatureValue[60]=10;
  temperatureValue[61]=10.5;
  temperatureValue[62]=11;
  temperatureValue[63]=11.5;
  temperatureValue[64]=12;
  temperatureValue[65]=12.5;
  temperatureValue[66]=13;
  temperatureValue[67]=13.5;
  temperatureValue[68]=14;
  temperatureValue[69]=14.5;
  temperatureValue[70]=15;
  temperatureValue[71]=15.5;
  temperatureValue[72]=16;
  temperatureValue[73]=16.5;
  temperatureValue[74]=17;
  temperatureValue[75]=17.5;
  temperatureValue[76]=18;
  temperatureValue[77]=18.5;
  temperatureValue[78]=19;
  temperatureValue[79]=19.5;
  temperatureValue[80]=20;
  temperatureValue[81]=20.5;
  temperatureValue[82]=21;
  temperatureValue[83]=21.5;
  temperatureValue[84]=22;
  temperatureValue[85]=22.5;
  temperatureValue[86]=23;
  temperatureValue[87]=23.5;
  temperatureValue[88]=24;
  temperatureValue[89]=24.5;
  temperatureValue[90]=25;
  temperatureValue[91]=25.5;
  temperatureValue[92]=26;
  temperatureValue[93]=26.5;
  temperatureValue[94]=27;
  temperatureValue[95]=27.5;
  temperatureValue[96]=28;
  temperatureValue[97]=28.5;
  temperatureValue[98]=29;
  temperatureValue[99]=29.5;
  temperatureValue[100]=30;
  temperatureValue[101]=30.5;
  temperatureValue[102]=31;
  temperatureValue[103]=31.5;
  temperatureValue[104]=32;
  temperatureValue[105]=32.5;
  temperatureValue[106]=33;
  temperatureValue[107]=33.5;
  temperatureValue[108]=34;
  temperatureValue[109]=34.5;
  temperatureValue[110]=35;
  temperatureValue[111]=35.5;
  temperatureValue[112]=36;
  temperatureValue[113]=36.5;
  temperatureValue[114]=37;
  temperatureValue[115]=37.5;
  temperatureValue[116]=38;
  temperatureValue[117]=38.5;
  temperatureValue[118]=39;
  temperatureValue[119]=39.5;
  temperatureValue[120]=40;
  temperatureValue[121]=40.5;
  temperatureValue[122]=41;
  temperatureValue[123]=41.5;
  temperatureValue[124]=42;
  temperatureValue[125]=42.5;
  temperatureValue[126]=43;
  temperatureValue[127]=43.5;
  temperatureValue[128]=44;
  temperatureValue[129]=44.5;
  temperatureValue[130]=45;
  temperatureValue[131]=45.5;
  temperatureValue[132]=46;
  temperatureValue[133]=46.5;
  temperatureValue[134]=47;
  temperatureValue[135]=47.5;
  temperatureValue[136]=48;
  temperatureValue[137]=48.5;
  temperatureValue[138]=49;
  temperatureValue[139]=49.5;
  temperatureValue[140]=50;
  temperatureValue[141]=50.5;
  temperatureValue[142]=51;
  temperatureValue[143]=51.5;
  temperatureValue[144]=52;
  temperatureValue[145]=52.5;
  temperatureValue[146]=53;
  temperatureValue[147]=53.5;
  temperatureValue[148]=54;
  temperatureValue[149]=54.5;
  temperatureValue[150]=55;
  temperatureValue[151]=55.5;
  temperatureValue[152]=56;
  temperatureValue[153]=56.5;
  temperatureValue[154]=57;
  temperatureValue[155]=57.5;
  temperatureValue[156]=58;
  temperatureValue[157]=58.5;
  temperatureValue[158]=59;
  temperatureValue[159]=59.5;
  temperatureValue[160]=60;
  temperatureValue[161]=60.5;
  temperatureValue[162]=61;
  temperatureValue[163]=61.5;
  temperatureValue[164]=62;
  temperatureValue[165]=62.5;
  temperatureValue[166]=63;
  temperatureValue[167]=63.5;
  temperatureValue[168]=64;
  temperatureValue[169]=64.5;
  temperatureValue[170]=65;
  temperatureValue[171]=65.5;
  temperatureValue[172]=66;
  temperatureValue[173]=66.5;
  temperatureValue[174]=67;
  temperatureValue[175]=67.5;
  temperatureValue[176]=68;
  temperatureValue[177]=68.5;
  temperatureValue[178]=69;
  temperatureValue[179]=69.5;
  temperatureValue[180]=70;
  temperatureValue[181]=70.5;
  temperatureValue[182]=71;
  temperatureValue[183]=71.5;
  temperatureValue[184]=72;
  temperatureValue[185]=72.5;
  temperatureValue[186]=73;
  temperatureValue[187]=73.5;
  temperatureValue[188]=74;
  temperatureValue[189]=74.5;
  temperatureValue[190]=75;
  temperatureValue[191]=75.5;
  temperatureValue[192]=76;
  temperatureValue[193]=76.5;
  temperatureValue[194]=77;
  temperatureValue[195]=77.5;
  temperatureValue[196]=78;
  temperatureValue[197]=78.5;
  temperatureValue[198]=79;
  temperatureValue[199]=79.5;
  temperatureValue[200]=80;
  temperatureValue[201]=80.5;
  temperatureValue[202]=81;
  temperatureValue[203]=81.5;
  temperatureValue[204]=82;
  temperatureValue[205]=82.5;
  temperatureValue[206]=83;
  temperatureValue[207]=83.5;
  temperatureValue[208]=84;
  temperatureValue[209]=84.5;
  temperatureValue[210]=85;
  temperatureValue[211]=85.5;
  temperatureValue[212]=86;
  temperatureValue[213]=86.5;
  temperatureValue[214]=87;
  temperatureValue[215]=87.5;
  temperatureValue[216]=88;
  temperatureValue[217]=88.5;
  temperatureValue[218]=89;
  temperatureValue[219]=89.5;
  temperatureValue[220]=90;
  temperatureValue[221]=90.5;
  temperatureValue[222]=91;
  temperatureValue[223]=91.5;
  temperatureValue[224]=92;
  temperatureValue[225]=92.5;
  temperatureValue[226]=93;
  temperatureValue[227]=93.5;
  temperatureValue[228]=94;
  temperatureValue[229]=94.5;
  temperatureValue[230]=95;
  temperatureValue[231]=95.5;
  temperatureValue[232]=96;
  temperatureValue[233]=96.5;
  temperatureValue[234]=97;
  temperatureValue[235]=97.5;
  temperatureValue[236]=98;
  temperatureValue[237]=98.5;
  temperatureValue[238]=99;
  temperatureValue[239]=99.5;
  temperatureValue[240]=100;
  temperatureValue[241]=1000;
  rValue[0]=95337;
  rValue[1]=92655.9;
  rValue[2]=90058;
  rValue[3]=87540.6;
  rValue[4]=85100.9;
  rValue[5]=82736.4;
  rValue[6]=80444.5;
  rValue[7]=78222.7;
  rValue[8]=76068.9;
  rValue[9]=73980.6;
  rValue[10]=71955.8;
  rValue[11]=69992.3;
  rValue[12]=68088.1;
  rValue[13]=66241.2;
  rValue[14]=64449.9;
  rValue[15]=62712.2;
  rValue[16]=61026.4;
  rValue[17]=59390.8;
  rValue[18]=57803.8;
  rValue[19]=56263.9;
  rValue[20]=54769.4;
  rValue[21]=53318.9;
  rValue[22]=51911.1;
  rValue[23]=50544.5;
  rValue[24]=49217.8;
  rValue[25]=47929.8;
  rValue[26]=46679.2;
  rValue[27]=45464.9;
  rValue[28]=44285.6;
  rValue[29]=43140.3;
  rValue[30]=42027.9;
  rValue[31]=40947.4;
  rValue[32]=39897.8;
  rValue[33]=38878;
  rValue[34]=37887.3;
  rValue[35]=36924.6;
  rValue[36]=35989.2;
  rValue[37]=35080.1;
  rValue[38]=34196.5;
  rValue[39]=33337.8;
  rValue[40]=32503;
  rValue[41]=31691.5;
  rValue[42]=30902.6;
  rValue[43]=30135.5;
  rValue[44]=29389.6;
  rValue[45]=28664.4;
  rValue[46]=27959;
  rValue[47]=27273;
  rValue[48]=26605.8;
  rValue[49]=25956.7;
  rValue[50]=25325.4;
  rValue[51]=24711.1;
  rValue[52]=24113.5;
  rValue[53]=23532;
  rValue[54]=22966.1;
  rValue[55]=22415.4;
  rValue[56]=21879.5;
  rValue[57]=21357.9;
  rValue[58]=20850.2;
  rValue[59]=20355.9;
  rValue[60]=19874.7;
  rValue[61]=19406.3;
  rValue[62]=18950.2;
  rValue[63]=18506;
  rValue[64]=18073.5;
  rValue[65]=17652.3;
  rValue[66]=17242.1;
  rValue[67]=16842.6;
  rValue[68]=16453.4;
  rValue[69]=16074.3;
  rValue[70]=15704.9;
  rValue[71]=15345;
  rValue[72]=14994.4;
  rValue[73]=14652.8;
  rValue[74]=14319.8;
  rValue[75]=13995.4;
  rValue[76]=13679.2;
  rValue[77]=13371;
  rValue[78]=13070.5;
  rValue[79]=12777.7;
  rValue[80]=12492.2;
  rValue[81]=12213.8;
  rValue[82]=11942.5;
  rValue[83]=11677.8;
  rValue[84]=11419.8;
  rValue[85]=11168.1;
  rValue[86]=10922.7;
  rValue[87]=10683.4;
  rValue[88]=10449.9;
  rValue[89]=10222.2;
  rValue[90]=10000;
  rValue[91]=9783.3;
  rValue[92]=9571.8;
  rValue[93]=9365.5;
  rValue[94]=9164.2;
  rValue[95]=8967.7;
  rValue[96]=8776;
  rValue[97]=8588.9;
  rValue[98]=8406.3;
  rValue[99]=8228.1;
  rValue[100]=8054.1;
  rValue[101]=7884.2;
  rValue[102]=7718.4;
  rValue[103]=7556.5;
  rValue[104]=7398.5;
  rValue[105]=7244.2;
  rValue[106]=7093.5;
  rValue[107]=6946.3;
  rValue[108]=6802.6;
  rValue[109]=6662.2;
  rValue[110]=6525.1;
  rValue[111]=6391.2;
  rValue[112]=6260.4;
  rValue[113]=6132.6;
  rValue[114]=6007.7;
  rValue[115]=5885.8;
  rValue[116]=5766.6;
  rValue[117]=5650.1;
  rValue[118]=5536.3;
  rValue[119]=5425.1;
  rValue[120]=5316.4;
  rValue[121]=5210.2;
  rValue[122]=5106.4;
  rValue[123]=5004.9;
  rValue[124]=4905.7;
  rValue[125]=4808.8;
  rValue[126]=4714;
  rValue[127]=4621.3;
  rValue[128]=4530.7;
  rValue[129]=4442.1;
  rValue[130]=4355.4;
  rValue[131]=4270.7;
  rValue[132]=4187.8;
  rValue[133]=4106.8;
  rValue[134]=4027.5;
  rValue[135]=3950;
  rValue[136]=3874.2;
  rValue[137]=3800;
  rValue[138]=3727.5;
  rValue[139]=3656.5;
  rValue[140]=3587;
  rValue[141]=3519;
  rValue[142]=3452.5;
  rValue[143]=3387.5;
  rValue[144]=3323.8;
  rValue[145]=3261.5;
  rValue[146]=3200.5;
  rValue[147]=3140.8;
  rValue[148]=3082.4;
  rValue[149]=3025.2;
  rValue[150]=2969.2;
  rValue[151]=2914.4;
  rValue[152]=2860.8;
  rValue[153]=2808.2;
  rValue[154]=2756.8;
  rValue[155]=2706.5;
  rValue[156]=2657.2;
  rValue[157]=2608.9;
  rValue[158]=2561.6;
  rValue[159]=2515.3;
  rValue[160]=2470;
  rValue[161]=2425.5;
  rValue[162]=2382;
  rValue[163]=2339.4;
  rValue[164]=2297.7;
  rValue[165]=2256.8;
  rValue[166]=2216.7;
  rValue[167]=2177.5;
  rValue[168]=2139;
  rValue[169]=2101.3;
  rValue[170]=2064.4;
  rValue[171]=2028.2;
  rValue[172]=1992.8;
  rValue[173]=1958;
  rValue[174]=1924;
  rValue[175]=1890.6;
  rValue[176]=1857.9;
  rValue[177]=1825.8;
  rValue[178]=1794.4;
  rValue[179]=1763.6;
  rValue[180]=1733.4;
  rValue[181]=1703.7;
  rValue[182]=1674.7;
  rValue[183]=1646.2;
  rValue[184]=1618.3;
  rValue[185]=1591;
  rValue[186]=1564.1;
  rValue[187]=1537.8;
  rValue[188]=1512;
  rValue[189]=1486.7;
  rValue[190]=1461.9;
  rValue[191]=1437.5;
  rValue[192]=1413.6;
  rValue[193]=1390.2;
  rValue[194]=1367.2;
  rValue[195]=1344.7;
  rValue[196]=1322.5;
  rValue[197]=1300.8;
  rValue[198]=1279.5;
  rValue[199]=1258.6;
  rValue[200]=1238.1;
  rValue[201]=1218;
  rValue[202]=1198.3;
  rValue[203]=1178.9;
  rValue[204]=1159.9;
  rValue[205]=1141.2;
  rValue[206]=1122.9;
  rValue[207]=1105;
  rValue[208]=1087.3;
  rValue[209]=1070;
  rValue[210]=1053;
  rValue[211]=1036.3;
  rValue[212]=1019.9;
  rValue[213]=1003.8;
  rValue[214]=988;
  rValue[215]=972.5;
  rValue[216]=957.3;
  rValue[217]=942.4;
  rValue[218]=927.7;
  rValue[219]=913.3;
  rValue[220]=899.1;
  rValue[221]=885.2;
  rValue[222]=871.5;
  rValue[223]=858.1;
  rValue[224]=845;
  rValue[225]=832;
  rValue[226]=819.3;
  rValue[227]=806.8;
  rValue[228]=794.5;
  rValue[229]=782.5;
  rValue[230]=770.7;
  rValue[231]=759;
  rValue[232]=747.6;
  rValue[233]=736.4;
  rValue[234]=725.3;
  rValue[235]=714.5;
  rValue[236]=703.8;
  rValue[237]=693.3;
  rValue[238]=683.1;
  rValue[239]=672.9;
  rValue[240]=663;
  rValue[241]=0;
}


double getTemperature(double vData) {
  double vValue = vData / (double)1024;
  double calculatedRValue = vValue * (double)Rs / ((double)Vcc - vValue);
  for(int i = 241; i >= 0; i--) {
    double lowerR = rValue[i];
    double higherR = rValue[i - 1];
    // Serial.print(i);
    // Serial.print(":low:");
    // Serial.print(lowerR);
    // Serial.print("high:");
    // Serial.println(higherR);
    if (calculatedRValue >= lowerR && calculatedRValue < higherR) {
      return temperatureValue[i];
    }
  }
  return -1;
}