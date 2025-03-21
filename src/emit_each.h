#ifndef EMIT_EACH_src
#define EMIT_EACH_src

#include <Arduino.h>

void emit_each_setup();

void emit_led_digit(uint8_t value);

void emit_num(uint8_t digit, uint8_t num);

void emit_refresh();

void emit_show_colon(bool keep_off = false);

void emit_clear_digit(uint8_t digit);

#endif
