#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <vector>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "TM1637.h"

#define CLK 26//pins definitions for TM1637 and can be changed to other ports       
#define DIO 27
TM1637 tm1637(CLK,DIO);
// Пин для подключения DS18B20
#define ONE_WIRE_BUS 25

// Настройка объектов для работы с датчиком
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress sensor = {0x28, 0xE6, 0xD4, 0x80, 0xE3, 0xE1, 0x3C, 0x92};

const int LED_NUMBER = 3;
const char *ssid = "POCOX3Pro"; 
const char *password = "O98765421"; 
const char *serverUrl = "http://192.168.161.200/Ubuntu_server/api.php";

const int ledPins[] = {14, 23, 33};
std::vector<int> cells;

const int trigPins[] = {13, 22, 19};
const int echoPins[] = {12, 18, 21};
float lastDistances[3] = {0, 0, 0};

void readTemperatureTask(void *pvParameters) {
  while (1) {
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);

    Serial.print(">>>>>>Температура: ");
    Serial.print(tempC);
    Serial.println(" °C");

    vTaskDelay(15000 / portTICK_PERIOD_MS);  // Задержка 1 секунда (FreeRTOS-совместимая)
  }
}

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
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);
  sensors.begin();

  xTaskCreatePinnedToCore(
    readTemperatureTask,  // Функция задачи
    "TemperatureTask",    // Название задачи (для отладки)
    10000,                // Размер стека (можно увеличить, если нужно)
    NULL,                 // Параметры задачи (не используем)
    1,                    // Приоритет (1 - стандартный)
    NULL,                 // Дескриптор задачи (не сохраняем)
    0               // Ядро (0 - Core 0)
  );
  
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
    //if ((distance - lastDistances[i]) > 1.0) {
      lastDistances[i] = distance;
      Serial.print("Датчик ");
      Serial.print(i+1);
      Serial.print(": ");
      Serial.print(distance);
      Serial.println(" см");
    //}


      HTTPClient http;
      http.begin("http://192.168.161.200/Ubuntu_server/get_sensors.php");
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int httpResCode = http.POST("sensor_id=" + String(i+1) + "&value=" + String(distance));
      Serial.println("sensor_id=" + String(i+1) + "&value=" + String(distance));
      ///Serial.print("HTTP RESPONSE: ");
      ///Serial.println(httpResCode);
      http.end();
      delay(5000);
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
