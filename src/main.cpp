//#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
//#include <FS.h>

//declaring an object of type asyncwebserver to control functions related to server defining port no 80 for server to listen to
AsyncWebServer server(80);
//defining the web socket route
AsyncWebSocket ws("/chat");

//initiailizing variables to hold wifi credentials
const char* ssid = "mywifi";
const char* password = "12345678";



unsigned long previousMillis = 0;
unsigned long interval = 30000;

//function for handling the websocket clients (connection, disconnection, and forwarding received data to all the clients connected)
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){
  if(type == WS_EVT_CONNECT){
    Serial.println("Websocket client Connection Established");
  }
  else if(type == WS_EVT_DISCONNECT){
    Serial.println("Client Disconnected");
  }
  else if(type == WS_EVT_DATA){
    ws.textAll(data, len);
    Serial.println("Dtata Received: ");
    for(int i=0; i<len; i++)
      Serial.print((char) data[i]);
  }
  Serial.println();
}


void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  // Wait for wifi to be connected
 uint32_t notConnectedCounter = 0;
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      notConnectedCounter++;
      if(notConnectedCounter > 15) { // Reset board if not connected after 5s
          Serial.println("Resetting due to Wifi not connecting...");
          ESP.restart();
      }
  }
  Serial.print("Wifi connected, IP address: ");
  Serial.println(WiFi.localIP());


//after connecting to wifi 
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  server.begin();

}


void loop() {
    unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=interval)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
  }
}

