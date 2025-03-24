#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "S23 FE"; 
const char* password = "j7squ2iqnuity4k"; 
const char* serverUrl = "http://192.168.1.40/api.php"; 

const int ledPin = 2; // Пин, к которому подключён светодиод

void setup() {
  Serial.begin(115200); 
  pinMode(ledPin, OUTPUT); 
  WiFi.begin(ssid, password); 

  // Ожидание подключения к Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); 
    Serial.println("Connecting to WiFi..."); 
  }
  Serial.println("Connected to WiFi"); 
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http; 

    //URL-запрос
    String url = String(serverUrl);
    Serial.print("Requesting URL: ");
    Serial.println(url); 

    http.begin(url); 

    int httpCode = http.GET(); 

    if (httpCode == HTTP_CODE_OK) { 
      String payload = http.getString(); 
      Serial.println("Response payload: ");
      Serial.println(payload); 

      // Парсинг JSON
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);

      if (error) { 
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str()); 
      } else {
        int param1 = doc["param1"]; 
        Serial.print("param1: ");
        Serial.println(param1); /

        if (param1 == 666) {
          for (int i = 0; i < 5; i++) {
            digitalWrite(ledPin, HIGH); 
            delay(500); 
            digitalWrite(ledPin, LOW);  
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
  delay(5000); 
}