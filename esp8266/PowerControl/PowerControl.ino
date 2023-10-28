#include <Arduino.h>
#include "EmonLib.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

EnergyMonitor emon1;

const char* ssid = "SSID";
const char* password = "PASSWORD";


ESP8266WebServer server(80);

const float voltage = 120.0; // Change this to your actual voltage (in volts)

void setup() {
  Serial.begin(115200);
  emon1.current(A0, 30); // Use A0 for the analog pin
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to the WiFi network");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/type", HTTP_GET, handleType);
  server.begin();
  Serial.println("Server started");
}

void handleRoot() {
  double amps = emon1.calcIrms(1480); 
  double watts = voltage * amps; // Calculate power in watts

  // Create a JSON response with current (Amps) and power (Watts)
  server.send(200, "text/plain", String(watts, 2));
}

void handleType() {
  server.send(200, "text/plain", "3");
}

void loop() {
  server.handleClient();
}
