#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "SSID";
const char* password = "PASSWORD";

ESP8266WebServer server(80);

const int DIGITAL_PIN = 5; // Pin digital utilizado para la verificación

void setup() {
  Serial.begin(115200);

  pinMode(DIGITAL_PIN, INPUT_PULLUP); // Configurar el pin digital como entrada con resistencia pull-up

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conectado a la red WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);

  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  int pinState = digitalRead(DIGITAL_PIN);
  if (pinState == HIGH) {
    server.send(200, "text/plain", "on");
  } else {
    server.send(200, "text/plain", "OFF");
  }
}
