#include <Adafruit_Fingerprint.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "ECOSMART";
const char* password = "netflix.com";

ESP8266WebServer server(80);


#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(D1, D2);

#else
#define mySerial Serial1
#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conectado a la red WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
  server.on("/finger", handleRoot);
  server.on("/type", handleType);
  server.begin();
  Serial.println("Servidor iniciado");
  while (!Serial);  
  delay(100);
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }
  finger.getParameters();
  finger.getTemplateCount();
  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  } else {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains ");
    Serial.print(finger.templateCount);
    Serial.println(" templates");
  }

}

void loop() {
  server.handleClient();
}

int getFingerprintID() {
  int fingerprintID = -1; // Initialize with an invalid value

  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) {
    return fingerprintID; // Return the initial invalid value
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) {
    return fingerprintID; // Return the initial invalid value
  }

  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.print("Found ID #");
    Serial.println(finger.fingerID);
    fingerprintID = finger.fingerID; // Update the fingerprint ID
  }

  return fingerprintID; // Return the fingerprint ID or the initial invalid value
}


void handleRoot() {
  int fing = getFingerprintID();
  Serial.println(fing);
  if (fing){
    server.send(200, "text/plain", String(fing));
  }
  else{
    server.send(200, "text/plain", "NaN"); 
  }
}
void handleType() {
  server.send(200, "text/plain", "4");
}
