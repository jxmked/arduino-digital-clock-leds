#ifndef ENV_h
#define ENV_h

#include <Arduino.h>

#include "DOTENV.h"

const uint8_t MAC_ADDR[6] = {0xDE, 0xAD, 0xBE, 0xBA, 0xDA, 0x55};

struct WIFI_INTERVAL {
  int TURN_ON = (60 * 60 * 12) * 1000;  // 12 Hours
  int KEEP_ON = (60 * 2) * 1000;  // 2 Minutes. While no connection to be found
};

struct SWITCH_TIME_DATE {
  int TIME_ON = 3000;
  int DATE_ON = 2000;
};

const char DATE_TIME_API_URL[] =
    "https://timeapi.io/api/time/current/zone?timeZone=Asia/Manila";

#endif