#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "S23 FE"; 
const char* password = "j7squ2iqnuity4k"; 
const char* serverUrl = "http://192.168.1.40/api.php"; 

const int ledPins[] = {41, 42, 43}; // Пины для 3 светодиодов (номера лучше уточнить)

// Пины для HC-SR04
const int trigPin = 12;  // Триггерный пин
const int echoPin = 14;  // Эхо-пин

void setup() {
  Serial.begin(115200);
  
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);

    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Response payload: " + payload);

      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
      } else {
        int param1 = doc["param1"];
        int cell = doc["cell"];

        Serial.print("param1: ");
        Serial.println(param1);
        Serial.print("cell: ");
        Serial.println(cell);

        if (cell >= 1 && cell <= 3) {
          int ledIndex = cell - 1;
          for (int i = 0; i < 5; i++) {
            digitalWrite(ledPins[ledIndex], HIGH);
            delay(500);
            digitalWrite(ledPins[ledIndex], LOW);
            delay(500);
          }
        }
      }
    } else {
      Serial.print("HTTP request failed, error code: ");
      Serial.println(httpCode);
    }
    http.end();
  } else {
    Serial.println("WiFi disconnected");
  }
  
  // Измерение расстояния с HC-SR04
  long duration;
  float distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.0343) / 2; // Преобразование времени в расстояние

  Serial.print("Расстояние: ");
  Serial.print(distance);
  Serial.println(" см");
  
  delay(5000);
}
