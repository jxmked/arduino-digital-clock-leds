#ifndef EMIT_EACH_src
#define EMIT_EACH_src

#include <Arduino.h>

void emit_each_setup();

void _set_all_led_data(int value);

void _emit_s_led(int index, int mode);

void _emit_arr_led(int pins[7], int count, uint8_t state);

void emit_num(int seg, int num);

void emit_led(int seg, int index);

void emit_week(int index);

void emit_util(int index);

#endif