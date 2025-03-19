#ifndef EMIT_EACH_src
#define EMIT_EACH_src

#include <Arduino.h>

void emit_each_setup();

void emit_led_digit(uint8_t value);

void emit_num(uint8_t seg, uint8_t num);

void emit_refresh();

#endif