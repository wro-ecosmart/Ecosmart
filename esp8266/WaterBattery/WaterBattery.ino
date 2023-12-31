#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "SSID";
const char* password = "PASSWORD";

ESP8266WebServer server(80);
const int pin = A0; // Pin analógico
const int pinValv1 = 12;
const int pinBom1 = 13;
const int pinBom2 = 14;


void setup() {
  Serial.begin(115200);
  pinMode(pinValv1, OUTPUT);
  pinMode(pinBom1, OUTPUT);
  pinMode(pinBom2, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conectado a la red WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/type", handleType);
  server.on("/1/on", onValv1);
  server.on("/1/off", offValv1);
  server.on("/2/on", onBom1);
  server.on("/2/off", offBom1);
  server.on("/3/on", onBom2);
  server.on("/3/off", offBom2);
 
  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  float water = readAnalog();
  server.send(200, "text/plain", String(water));
}

void handleType() {
  server.send(200, "text/plain", "2");
}

float readAnalog() {
  float rs = 0;
  rs = analogRead(pin);
  delay(100);
  return rs;
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

void onBom2() {
  digitalWrite(pinBom2, HIGH);
  server.send(200, "text/plain", "ok");
}

void offBom2() {
  digitalWrite(pinBom2, LOW);
  server.send(200, "text/plain", "ok");
}
