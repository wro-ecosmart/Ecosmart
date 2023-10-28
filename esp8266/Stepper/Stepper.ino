#include <WiFi.h>
#include <Stepper.h>

bool Isrunning = HIGH;
const int stepsPerRevolution = 300;  // Ajusta esto según el número de pasos por revolución de tu motor.

Stepper myStepper(stepsPerRevolution, 12, 14, 27, 26);

const char* ssid = "SSID";
const char* password = "PASSWORD";

WiFiServer server(80);

void setup() {
  myStepper.setSpeed(60);
  Serial.begin(9600);
  WiFi.begin(ssid, password);  // Aquí se pasan como punteros
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conectado a la red WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  if (Isrunning) {
    myStepper.step(stepsPerRevolution);
  }

  WiFiClient client = server.available();

  if (client) {
    if (client.connected()) {
      String request = client.readStringUntil('\r');
      if (request.indexOf("/off") != -1) {
        Isrunning = LOW;
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/plain");
        client.println();
        client.println("ok");
      } else if (request.indexOf("/on") != -1) {
        Isrunning = HIGH;
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/plain");
        client.println();
        client.println("ok");
      }
      client.stop();
    }
  }
}
