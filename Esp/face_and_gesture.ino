

// this code contains the esp control of the leds using the face and gesture detection 
// this code should be integrated with a python code for geesture detection and face for sending the signal via localhost
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Net1";       // Replace with your network credentials
const char* password = "hisham123";

WebServer server(80);

const int ledPins[] = {2, 4, 5, 18, 19};  // GPIO pins for the LEDs

void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  for (int i = 0; i < 5; i++) {
    server.on("/led_" + String(i + 1), [i]() {
      digitalWrite(ledPins[i], HIGH);  // Turn LED i+1 on
      server.send(200, "text/plain", "LED " + String(i + 1) + " is on");
    });
  }

  server.on("/led_off", []() {
    for (int i = 0; i < 5; i++) {
      digitalWrite(ledPins[i], LOW);  
    }
    server.send(200, "text/plain", "All LEDs are off");
  });

  server.on("/face_detected", []() {
    digitalWrite(ledPins[0], HIGH);
    digitalWrite(ledPins[1], HIGH);  
    digitalWrite(ledPins[2], HIGH);  
    digitalWrite(ledPins[3], HIGH);  
    digitalWrite(ledPins[4], HIGH);    
    server.send(200, "text/plain", "Face detected, LED 1 is on");
  });

  server.on("/no_face_detected", []() {
    digitalWrite(ledPins[0], LOW);
    digitalWrite(ledPins[1], LOW);
    digitalWrite(ledPins[2], LOW);
    digitalWrite(ledPins[3], LOW);
    digitalWrite(ledPins[4], LOW);
      // Turn LED 1 off
    server.send(200, "text/plain", "No face detected, LED 1 is off");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
