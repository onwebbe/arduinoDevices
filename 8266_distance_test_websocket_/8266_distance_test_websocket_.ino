#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <WebSocketsServer.h>
const char* ssid = "Drive";
const char* password = "1234554321";

WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  IPAddress local_IP(192,168,5,1);//手动设置的开启的网络的ip地址
  IPAddress gateway(192,168,5,1);  //手动设置的网关IP地址
  IPAddress subnet(255,255,255,0); //手动设置的子网掩码
  //设置为接入点模式
  WiFi.mode(WIFI_AP);
  
   //配置接入点的IP，网关IP，子网掩码
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid, password);
  // 启动校验式网络（需要输入账号密码的网络）,通道为3，wifi隐藏，最大连接数=4
  // WiFi.softAP(AP_ssid, password,3,1);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/", []() {
    server.send(200, "text/html", getHTML());
  });
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}
uint8_t clientNum = -1;
unsigned long currentTime = millis();
int count = 0;
void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  webSocket.loop();
  if ((millis() - currentTime) >= 1000) {
    currentTime = millis();
    if (currentTime >= 0) {
      char test2[25] ;
      dtostr(test2,count++);
      webSocket.sendTXT(clientNum, test2);
    }
  }
}
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                clientNum = num;
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            }
            break;
        case WStype_TEXT:
            {
              Serial.printf("[%u] get Text: %s\n", num, payload);
              
            }
            break;
        case WStype_BIN:
            Serial.printf("[%u] get binary length: %u\n", num, length);
            hexdump(payload, length);

            // send message to client
            // webSocket.sendBIN(num, payload, length);
            break;
    }

}
char* dtostr(char *str, double d)
{
    sprintf(str, "%f", d);
    return str;
}
String getHTML() {
  String s = "<html>";
  s.concat("  <head>");
  s.concat("    <script>");
  s.concat("      var lastTime = 0;");
  s.concat("      var lastCount = 0;");
  s.concat("      var lostCount = 0;");
  s.concat("      var ws = new WebSocket(\"ws://192.168.5.1:81\");");
  s.concat("      ws.onopen = function(evt) { ");
  s.concat("        console.log(\"Connection open ...\");");
  s.concat("        document.getElementById('connect_status').innerHTML = 'connected';");
  s.concat("      };");
  s.concat("      ");
  s.concat("      ws.onclose = function(evt) {");
  s.concat("        console.log(\"Connection closed.\");");
  s.concat("        document.getElementById('connect_status').innerHTML = 'closed';");
  s.concat("      };");
  s.concat("");
  s.concat("      ws.onmessage = function(evt) {");
  s.concat("        console.log(\"Connection closed.\");");
  s.concat("        document.getElementById('connect_status').innerHTML = 'closed';");
  s.concat("      };");
  s.concat("");
  s.concat("      ws.onmessage = function(evt) {");
  s.concat("        console.log( \"Received Message: \" + evt.data);");
  s.concat("        document.getElementById('data_gap').innerHTML = (new Date().getTime() - lastTime);");
  s.concat("        document.getElementById('data_received').innerHTML = evt.data;");
  s.concat("        document.getElementById('lost_count').innerHTML = lostCount;");
  s.concat("        if (evt.data != (lastCount + 1)) {");
  s.concat("          lostCount = lostCount + 1;");
  s.concat("        }");
  s.concat("        lastCount = parseInt(evt.data);");
  s.concat("        lastTime = new Date().getTime();");
  s.concat("      };");
  s.concat("");
  s.concat("      ws.onerror = function(evt) {");
  s.concat("        console.log(\"error!!!\"); ");
  s.concat("        document.getElementById('connect_status').innerHTML = 'error';");
  s.concat("      };");
  s.concat("");
  s.concat("      function resetLostCount() {");
  s.concat("        lostCount = 0;");
  s.concat("      }");
  s.concat("    </script>");
  s.concat("  </head>");
  s.concat("  <body>");
  s.concat("    <div>status: <span id=\"connect_status\"></span></div>");
  s.concat("    <div>data: <span id=\"data_received\"></span></div>");
  s.concat("    <div>gap: <span id=\"data_gap\"></span></div>");
  s.concat("    <div>lostCount: <span id=\"lost_count\"></span></div>");
  s.concat("    <button onclick=\"resetLostCount();\">reset lost count</button>");
  s.concat("  </body>");
  s.concat("</html>");
  return s;
}
