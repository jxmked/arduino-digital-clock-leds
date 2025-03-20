
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

int state = 0;
int conn_attempt = 0;

void connect_setup() {
  WiFi.mode(WIFI_OFF);

  esp_base_mac_addr_set(MAC_ADDR);
  WiFi.setTxPower(WIFI_POWER_19_5dBm);
  WiFi.setHostname(stringify(WIFI_HOST_NAME));

  delay(1000);

  Serial.println("Wi-Fi initially off.");
}

UPDATE_TIME_CONST connect_loop() {
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
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWi-Fi connected!");
        state = 2;
      } else {
        Serial.print(".");

        conn_attempt++;
      }

      if (conn_attempt >= 20) {
        state = 3;  // jump to 3 instead of 2
      }

      break;

    case 2: {
      Serial.println("fetching time");
      conn_attempt = 0;
      state = 0;

      bool fetch_ok = connect_fetchAndParseJson();

      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);

      if (fetch_ok) {
        return UPDATE_TIME_CONST::OK;
      }
      return UPDATE_TIME_CONST::FAIL;
    } break;

    case 3:
      Serial.println("\nWi-Fi connection failed.");
      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);

      conn_attempt = 0;
      state = 0;

      return UPDATE_TIME_CONST::FAIL;

      break;

    default:
      break;
  }

  // return 1 so we can go back here and resume
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