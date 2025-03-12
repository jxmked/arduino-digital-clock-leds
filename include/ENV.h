#ifndef ENV_h
#define ENV_h

#include <Arduino.h>

#include "DOTENV.h"

struct WIFI_CONFIG {
  char *SSID = stringify(WIFI_SSID);
  char *PASSWORD = stringify(WIFI_PASS);
  char *HOST_NAME = stringify(WIFI_HOST_NAME);
  // uint8_t macaddress[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  uint8_t macaddress[6] = {0xDE, 0xAD, 0xBE, 0xBA, 0xDA, 0x55};
};

struct WIFI_INTERVAL {
  int TURN_ON = (60 * 60 * 12) * 1000;  // 12 Hours
  int KEEP_ON = (60 * 2) * 1000;  // 2 Minutes. While no connection to be found
};

const char DATE_TIME_API_URL[] =
    "https://timeapi.io/api/time/current/zone?timeZone=Asia/Manila";

#endif