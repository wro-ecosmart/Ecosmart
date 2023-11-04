#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "ECOSMART";
const char* password = "netflix.com";

ESP8266WebServer server(80);
const int pinValv1 = 12;
const int pinBom1 = 14;


void setup() {
  Serial.begin(115200);
  pinMode(pinValv1, OUTPUT);
  pinMode(pinBom1, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conectado a la red WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());


  server.on("/type", handleType);
  server.on("/1/on", onValv1);
  server.on("/1/off", offValv1);
  server.on("/2/on", onBom1);
  server.on("/2/off", offBom1);

 
  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  server.handleClient();
}



void handleType() {
  server.send(200, "text/plain", "2");
}


void onValv1() {
  digitalWrite(pinValv1, HIGH);
  server.send(200, "text/plain", "ok");
}

void offValv1() {
  digitalWrite(pinValv1, LOW);
  server.send(200, "text/plain", "ok");
}
void onBom1() {
  digitalWrite(pinBom1, HIGH);
  server.send(200, "text/plain", "ok");
}

void offBom1() {
  digitalWrite(pinBom1, LOW);
  server.send(200, "text/plain", "ok");
}
