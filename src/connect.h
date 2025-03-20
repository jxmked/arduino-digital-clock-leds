#ifndef CONNECT_h
#define CONNECT_h

#include <Arduino.h>

#include "TYPES.h"

bool connect_fetchAndParseJson();
void connect_setup();
UPDATE_TIME_CONST connect_loop();
void connect_update_time(_TIME_t *_cur_time);

#endif