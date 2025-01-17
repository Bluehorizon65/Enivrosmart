#include <SoftwareSerial.h>
#include <DHT.h>

// Pin definitions
#define LED1_PIN 3
#define LED2_PIN 4
#define DHT_PIN 2
#define RELAY_PIN 5 // Define the relay pin

// DHT setup
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

// Create a software serial port on pins 10 (RX) and 11 (TX)
SoftwareSerial BTSerial(10, 11); // RX, TX

void setup() {
  // Initialize hardware serial for the Arduino Serial Monitor
  Serial.begin(9600);
  // Initialize software serial for the Bluetooth module
  BTSerial.begin(9600);
  // Initialize DHT sensor
  dht.begin();

  // Set LED pins as output
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // Set relay pin as output
  pinMode(RELAY_PIN, OUTPUT);

  // Ensure LEDs and relay are off initially
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(RELAY_PIN, HIGH);

  Serial.println("HC-05 Bluetooth Module is ready to pair.");
}

void loop() {
  // Check if data is available from the Bluetooth module
  if (BTSerial.available()) {
    char c = BTSerial.read();
    Serial.print("Received: ");
    Serial.println(c);

    if (c == '1') {
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, LOW);
      Serial.println("LED1 ON");
    } else if (c == '2') {
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, HIGH);
      Serial.println("LED2 ON");
    } else if (c == '3') {
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, LOW);
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();

      if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
      } else {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" *C");
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");

        // Send temperature and humidity data back via Bluetooth
        BTSerial.print("Temperature: ");
        BTSerial.print(temperature);
        BTSerial.println(" *C");
        BTSerial.print("Humidity: ");
        BTSerial.print(humidity);
        BTSerial.println(" %");
      }
    } else if (c == '4') {
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, HIGH);
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();
      if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
      } else {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" *C");
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");

        BTSerial.print("Temperature: ");
        BTSerial.print(temperature);
        BTSerial.println(" *C");
        BTSerial.print("Humidity: ");
        BTSerial.print(humidity);
        BTSerial.println(" %");
      }
    } else if (c == '5') {
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, LOW);
    } else if (c == '6') {
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("Relay ON");
    } else if (c == '7') {
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println("Relay OFF");
    }
  }
}
