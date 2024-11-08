

/*
 Basic ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define DEVICE_NAME "esp8266_emqx_test_device"
#define HEADBEAT_CHANNEL "/device/light/8266homedevice/ESP8266Client-521b/beat"
#define SEND_CHANNEL "/device/light/8266homedevice/ESP8266Client-521b/send"
#define RECEIVE_CHANNEL "/device/light/8266homedevice/ESP8266Client-521b/receive"

#define LIGHT_PIN 16
// Update these with values suitable for your network.

const char* ssid = "pankey_asus";
const char* password = "1234554321";
const char* mqtt_server = "192.168.50.109";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    Serial.println("Switch OFF");
    digitalWrite(LIGHT_PIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    digitalWrite(BUILTIN_LED, HIGH);
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    Serial.println("Switch ON");
    digitalWrite(LIGHT_PIN, HIGH);  // Turn the LED off by making the voltage HIGH
    digitalWrite(BUILTIN_LED, LOW);
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-521b";
    // clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), "", "123456")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("homeassistant/switch/8266homedevice/ESP8266Client-521b/config", "{\"~\": \"/device/light/8266homedevice/ESP8266Client-521b\",\"name\": \"light-521b\",\"unique_id\": \"light-521b\",\"cmd_t\": \"~/receive\",\"stat_t\": \"~/beat\",\"pl_on\": \"0\",\"pl_off\": \"1\"}");
      // ... and resubscribe
      client.subscribe(RECEIVE_CHANNEL);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void heartBeat(PubSubClient *client) {
  String publishStr = "{\"status\":\"";
  publishStr.concat(client->state());
  publishStr.concat("\", \"time\":");
  publishStr.concat(millis());
  publishStr.concat("}");
  Serial.print("Publish heart beat: ");
  Serial.print(publishStr);
  Serial.println();
  client->publish(HEADBEAT_CHANNEL, publishStr.c_str());
}
void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(LIGHT_PIN, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(LIGHT_PIN, LOW);
  digitalWrite(BUILTIN_LED, LOW);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 9013);
  client.setCallback(callback);
  digitalWrite(BUILTIN_LED, HIGH);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;
    heartBeat(&client);
  }
}
