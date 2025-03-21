
#include "connect.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <TimeInterval.h>
#include <WiFi.h>
#include <esp_wifi.h>

#include "DOTENV.h"
#include "ENV.h"
#include "TYPES.h"

_TIME_t adjustable_time;

void connect_setup() {
  WiFi.mode(WIFI_OFF);

  esp_base_mac_addr_set(MAC_ADDR);
  WiFi.setTxPower(WIFI_POWER_19_5dBm);
  WiFi.setHostname(stringify(WIFI_HOST_NAME));

  delay(1000);
}

UPDATE_TIME_CONST connect_loop() {
  static uint8_t state = 0;
  static unsigned long conn_ival = 0;

  // Non-blocking ???
  switch (state) {
    case 0:
      WiFi.mode(WIFI_STA);
      WiFi.begin(stringify(WIFI_SSID), stringify(WIFI_PASS));

      Serial.printf("SSID: %s \n\n", stringify(WIFI_SSID));

      Serial.print("Connecting to Wi-Fi...");

      state = 1;
      break;

    case 1:
      if (conn_ival == 0) conn_ival = millis();

      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWi-Fi connected!");
        state = 2;

      } else if (millis() - conn_ival >= 10000) {
        // Wait for 10 seconds before
        // we declaire of failure
        Serial.println("\nWi-Fi connection failed.");

        state = 3;  // jump to 3 instead of 2
      }

      break;

    case 2: {
      Serial.println("Fetching time");
      conn_ival = 0;
      state = 0;

      bool fetch_ok = connect_fetchAndParseJson();

      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);

      if (fetch_ok) {
        Serial.println("Fetching success");
        return UPDATE_TIME_CONST::OK;
      }

      Serial.println("Fetching failed");
      return UPDATE_TIME_CONST::FAIL;
    };

    case 3:
      Serial.println("Closing Connection");

      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);

      conn_ival = 0;
      state = 0;

      return UPDATE_TIME_CONST::FAIL;
  }

  return UPDATE_TIME_CONST::CONTINUE;
}

bool connect_fetchAndParseJson() {
  HTTPClient http;

  http.begin(DATE_TIME_API_URL);

  int httpCode = http.GET();

  if (httpCode <= 0 || httpCode != HTTP_CODE_OK) {
    Serial.println("Failed to fetch data/time");
    http.end();

    return false;
  }

  String payload = http.getString();

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("JSON parsing failed: ");

    http.end();

    return false;
  }

  JsonObject obj = doc.as<JsonObject>();

  adjustable_time.hour = obj["hour"].as<const uint8_t>();
  adjustable_time.minute = obj["minute"].as<const uint8_t>();
  adjustable_time.day = obj["day"].as<const uint8_t>();
  adjustable_time.month = obj["month"].as<const uint8_t>();
  adjustable_time.year = obj["year"].as<const uint8_t>();

  http.end();

  return true;
}

void connect_update_time(_TIME_t* _cur_time) {
  _cur_time->hour = adjustable_time.hour;
  _cur_time->minute = adjustable_time.minute;

  _cur_time->day = adjustable_time.day;
  _cur_time->month = adjustable_time.month;
  _cur_time->year = adjustable_time.year;
}