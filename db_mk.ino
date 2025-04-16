#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <vector>

const int LED_NUMBER = 3;
const char *ssid = "S23 FE"; 
const char *password = "j7squ2iqnuity4k"; 
const char *serverUrl = "http://192.168.92.18/Ubuntu_server/api.php";

const int ledPins[] = {14, 23, 33};
std::vector<int> cells;

const int trigPins[] = {13, 32, 19};
const int echoPins[] = {12, 35, 21};
float lastDistances[3] = {0, 0, 0};

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

float measureDistance(int sensorNum) {

  digitalWrite(trigPins[sensorNum], LOW);
  delayMicroseconds(2);
  digitalWrite(trigPins[sensorNum], HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPins[sensorNum], LOW);
  
  float duration = pulseIn(echoPins[sensorNum], HIGH);
  return (duration * 0.0343) / 2;
}

void setup() {
  Serial.begin(115200);
  
  for (int i = 0; i < 3; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }

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
  
  for (int i = 0; i < 3; i++) {
    float distance = measureDistance(i);
    if (abs(distance - lastDistances[i]) > 1.0) {
      lastDistances[i] = distance;
      Serial.print("Датчик ");
      Serial.print(i+1);
      Serial.print(": ");
      Serial.print(distance);
      Serial.println(" см");
    }

      HTTPClient http;
      http.begin("http://192.168.126.18/Ubuntu_server/get_sensors.php");
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      http.POST("sensor_id=" + String(i+1) + "&value=" + String(distance));
      http.end();
      delay(50);
  }

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

  delay(500);
}
