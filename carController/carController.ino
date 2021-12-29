#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "pankey_asus";
const char* password = "1234554321";

WebServer server(80);
#define PORT1 5;
#define PORT2 6;
#define PORT3 7;
#define PORT4 8;

//#define PORT1 25;
//#define PORT2 26;
//#define PORT3 32;
//#define PORT4 33;

const int led = 13;



void turnLeft() {
  digitalWrite(PORT1, LOW);
  digitalWrite(PORT2, LOW);
  digitalWrite(PORT4, HIGH);
  digitalWrite(PORT3, LOW);
  server.send(200, "text/plain", "MoveLeft");
}
void turnRight() {
  digitalWrite(PORT1, HIGH);
  digitalWrite(PORT2, LOW);
  digitalWrite(PORT4, LOW);
  digitalWrite(PORT3, LOW);
  server.send(200, "text/plain", "MoveRight");
}
void moveForward() {
  digitalWrite(PORT1, HIGH);
  digitalWrite(PORT2, LOW);
  digitalWrite(PORT4, HIGH);
  digitalWrite(PORT3, LOW);
  server.send(200, "text/plain", "MoveForward");
}
void moveBackward() {
  digitalWrite(PORT1, LOW);
  digitalWrite(PORT2, HIGH);
  digitalWrite(PORT4, LOW);
  digitalWrite(PORT3, HIGH);
  server.send(200, "text/plain", "MoveBackward");
}
void noMove() {
  digitalWrite(PORT1, LOW);
  digitalWrite(PORT2, LOW);
  digitalWrite(PORT4, LOW);
  digitalWrite(PORT3, LOW);
  server.send(200, "text/plain", "NoMove");
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(PORT1, OUTPUT);
  pinMode(PORT2, OUTPUT);
  pinMode(PORT3, OUTPUT);
  pinMode(PORT4, OUTPUT);
  delay(1000);


  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/api/car/direction/turnLeft", turnLeft);
  server.on("/api/car/direction/turnRight", turnRight);
  server.on("/api/car/direction/moveForward", moveForward);
  server.on("/api/car/direction/moveBackward", moveBackward);
  server.on("/api/car/direction/noMove", noMove);
//
//  server.on("/inline", []() {
//    server.send(200, "text/plain", "this works as well");
//  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
