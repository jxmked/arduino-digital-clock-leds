
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <TimeInterval.h>
#include <WiFi.h>
#include <esp_wifi.h>

#include <iostream>

#include "DOTENV.h"
#include "ENV.h"

unsigned long wifiOnInterval = 30000;  // 1 hour (3600000 milliseconds)
unsigned long wifiOnDuration = 30000;  // 2 minutes (120000 milliseconds)

unsigned long previousMillis = 0;
bool wifiEnabled = true;
unsigned long wifiStartTime = 0;

void fetchAndParseJson();

void connect_setup() {
  WiFi.mode(WIFI_OFF);

  esp_base_mac_addr_set(MAC_ADDR);
  WiFi.setTxPower(WIFI_POWER_19_5dBm);
  WiFi.setHostname(stringify(WIFI_HOST_NAME));

  delay(1000);

  Serial.println("Wi-Fi initially off.");
}

void connect_loop() {
  unsigned long currentMillis = millis();

  if (wifiEnabled) {
    if (currentMillis - wifiStartTime >= wifiOnDuration) {
      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
      wifiEnabled = false;
      Serial.println("Wi-Fi off.");
      previousMillis = currentMillis;
    }
  } else {
    if (currentMillis - previousMillis >= wifiOnInterval) {
      WiFi.mode(WIFI_STA);
      WiFi.begin(stringify(WIFI_SSID), stringify(WIFI_PASS));
      Serial.print("Connecting to Wi-Fi...");
      int attempts = 0;

      while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
      }

      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWi-Fi connected!");
        wifiEnabled = true;
        wifiStartTime = currentMillis;
        fetchAndParseJson();  // Fetch and parse JSON data
      } else {
        Serial.println("\nWi-Fi connection failed.");
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        previousMillis = currentMillis;
      }
    }
  }
  delay(1000);
}

void connect_fetchAndParseJson() {
  HTTPClient http;
  http.begin(DATE_TIME_API_URL);

  int httpCode = http.GET();

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("JSON Payload:");
      Serial.println(payload);

      DynamicJsonDocument doc(2048);  // Adjust size as needed
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
        return;
      }

      JsonObject obj = doc.as<JsonObject>();
      for (JsonPair pair : obj) {
        Serial.println(pair.key().c_str());
        // if (pair.value().isString())
        // {
        //   Serial.print("Key: ");
        //   Serial.print(pair.key().c_str());
        //   Serial.print(", Value (String): ");
        //   Serial.println(pair.value().as<String>());
        // }
        // else if (pair.value().isNumber())
        // {
        //   Serial.print("Key: ");
        //   Serial.print(pair.key().c_str());
        //   Serial.print(", Value (Number): ");
        //   Serial.println(pair.value().as<int>());
        // }
      }
    } else {
      Serial.printf("HTTP GET failed, code: %d\n", httpCode);
    }
  } else {
    Serial.println("HTTP GET failed, connection error");
  }

  http.end();
}