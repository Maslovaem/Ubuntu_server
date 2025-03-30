#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <vector>

const int LED_NUMBER = 3;

const char *ssid = "S23 FE"; 
const char *password = "j7squ2iqnuity4k"; 
const char *serverUrl = "http://192.168.95.103/Ubuntu_server/api.php"; 

const int ledPins[] = {25, 26, 27}; // Пины для 3 светодиодов
std::vector<int> cells;

//Пины для HC-SR04

/*const int trigPin = 12;  //Триггерный пин

const int echoPin = 14;  //Эхо-пин*/

void blink() {
  for (int i = 0; i < 5; i++) {
    
    for (auto it = cells.begin(); it != cells.end(); it++) {
      int cell = *it;
      if (cell >= 1 && cell <= 3) {
        digitalWrite(ledPins[cell - 1], HIGH);
      }
    }
    delay(500);
    
    for (auto it = cells.begin(); it != cells.end(); it++) {
      int cell = *it;
      if (cell >= 1 && cell <= 3) {
        digitalWrite(ledPins[cell - 1], LOW);
      }
    }
    delay(500);
  }
  cells.clear();
}

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < LED_NUMBER; i++) {
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
      payload.trim();
      Serial.println("Response payload: " + payload);

      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
      } else {
        JsonArray array = doc.is<JsonArray>() ? doc.as<JsonArray>() : doc.to<JsonArray>();

        for (JsonObject record : array) {
          String date = record["date"];  
          int cell = record["cell"];

          Serial.print("Date: ");
          Serial.print(date);
          Serial.print(" | Cell: ");
          Serial.println(cell);
          cells.push_back(cell);
        }
        blink();
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

  /*long duration;

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

  Serial.println(" см");*/

  delay(500);
}
