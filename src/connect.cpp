
#include "connect.h"

#include <Arduino.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <TimeInterval.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_wifi.h>

#include "DOTENV.h"
#include "ENV.h"
#include "TYPES.h"

_TIME_t adjustable_time;

bool update_success = false;

WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP);

void connect_setup() {
  WiFi.mode(WIFI_OFF);

  esp_base_mac_addr_set(MAC_ADDR);
  WiFi.setTxPower(WIFI_POWER_19_5dBm);
  WiFi.setHostname(stringify(WIFI_HOST_NAME));

  delay(1000);
}

void update_time_loop() { update_success = timeClient.update(); }

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
      timeClient.begin();
      timeClient.setTimeOffset(28800);

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
  update_time_loop();

  if (timeClient.isTimeSet()) {
    const auto epoch = timeClient.getEpochTime();

    epochToTime(epoch, adjustable_time);

    return true;
  }

  return false;
}

void connect_update_time(_TIME_t* _cur_time) {
  _cur_time->hour = adjustable_time.hour;
  _cur_time->minute = adjustable_time.minute;
  _cur_time->second = adjustable_time.second;

  _cur_time->day = adjustable_time.day;
  _cur_time->month = adjustable_time.month;
  _cur_time->year = adjustable_time.year;
}

bool isLeapYear(uint16_t year) {
  return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

void epochToTime(uint32_t epoch, _TIME_t& t) {
  const uint16_t daysInMonth[] = {31, 28, 31, 30, 31, 30,
                                  31, 31, 30, 31, 30, 31};

  // Time
  t.second = epoch % 60;
  epoch /= 60;
  t.minute = epoch % 60;
  epoch /= 60;
  t.hour = epoch % 24;
  epoch /= 24;  // now epoch = days since 1970-01-01

  // Date
  uint16_t year = 1970;
  while (true) {
    uint16_t daysInYear = isLeapYear(year) ? 366 : 365;
    if (epoch >= daysInYear) {
      epoch -= daysInYear;
      year++;
    } else {
      break;
    }
  }

  t.year = year % 100;  // store last two digits

  uint8_t month = 0;
  while (true) {
    uint8_t dim = daysInMonth[month];
    if (month == 1 && isLeapYear(year)) {
      dim = 29;
    }

    if (epoch >= dim) {
      epoch -= dim;
      month++;
    } else {
      break;
    }
  }

  t.month = month + 1;
  t.day = epoch + 1;
}
