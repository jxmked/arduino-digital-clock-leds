#ifndef EMIT_EACH_src
#define EMIT_EACH_src

#include <Arduino.h>

struct __emit_num_values {
  uint8_t leds[7];
  uint8_t led_count;
};

void emit_each_setup();

void _set_all_led_data(uint8_t value);

void _emit_s_led(uint8_t index, uint8_t mode);

void _emit_arr_led(const uint8_t pins[7], uint8_t count, uint8_t state);

void emit_num(uint8_t seg, uint8_t num);

void emit_led(uint8_t seg, uint8_t index);

void emit_week(uint8_t index);

void emit_util(uint8_t index, uint8_t state);

#endif