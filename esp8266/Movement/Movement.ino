#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


const char* ssid = "SSID";
const char* password = "PASSWORD";
boolean movimiento = false;
int inputPin = 5; // Pin D1 en Wemos D1 Mini


ESP8266WebServer server(80);

void handleRoot() {
  if (movimiento){
    server.send(200, "text/plain", "on");
  }else {
    server.send(200, "text/plain", "off");
  }
    
}

void setup() {
  pinMode(inputPin, INPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  int val = digitalRead(inputPin);
  if (val == HIGH) {
    movimiento = true;
    //Serial.println("Movimiento Detectado");
  } else {
    movimiento = false;
    //Serial.println("Movimiento no Detectado");
  }
}
