# this code will collect the soil moisture, temperature and humidity from the soil and send to the python code for predicting the soil quality



#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

#define DHTPIN 14      // Pin connected to the DHT sensor
#define DHTTYPE DHT11  // DHT 11
#define SOILMOISTUREPIN 34 // Pin connected to the soil moisture sensor

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

const char* ssid = ""; // ssid
const char* password = ""; // password

void setup() {
  Serial.begin(115200);
  
  // Initialize the DHT sensor
  dht.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Define the root route
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  // Read temperature and humidity from DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Read soil moisture level
  int soilMoistureValue = analogRead(SOILMOISTUREPIN);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    server.send(500, "text/plain", "Failed to read from DHT sensor");
    return;
  }

  // Create response message
  String response = "Temperature: " + String(t) + " C\n";
  response += "Humidity: " + String(h) + " %\n";
  response += "Soil Moisture: " + String(soilMoistureValue);

  // Send response
  server.send(200, "text/plain", response);
}
