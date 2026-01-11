#ifndef CONNECT_h
#define CONNECT_h

#include <Arduino.h>

#include "TYPES.h"

bool connect_fetchAndParseJson();
void connect_setup();
void update_time_loop();
UPDATE_TIME_CONST connect_loop();
void connect_update_time(_TIME_t* _cur_time);
bool isLeapYear(uint16_t year);

void epochToTime(uint32_t epoch, _TIME_t& t);

#endif