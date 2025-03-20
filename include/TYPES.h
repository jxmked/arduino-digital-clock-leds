#ifndef TYPES_h
#define TYPES_h

#include <Arduino.h>

struct _TIME_t {
  uint8_t hour;
  uint8_t minute;
  uint8_t day;
  uint8_t month;
  uint8_t year;
};

enum class UPDATE_TIME_CONST { CONTINUE, FAIL, OK };

#endif
