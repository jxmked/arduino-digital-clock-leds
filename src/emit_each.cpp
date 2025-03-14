#include "emit_each.h"

#include <Arduino.h>
#include <DEFINITION.h>

#include "LED_PIN.h"

__emit_num_values __emit_num_obj[10] = {
    {{0, 1, 2, 3, 4, 5}, 6}, {{2, 3}, 2},       {{1, 2, 4, 5, 6}, 5},
    {{1, 2, 3, 4, 6}, 5},    {{0, 2, 3, 6}, 4}, {{0, 1, 3, 4, 6}, 5},
    {{0, 1, 3, 4, 5, 6}, 6}, {{1, 2, 3}, 3},    {{0, 1, 2, 3, 4, 5, 6}, 7},
    {{0, 1, 2, 3, 4, 6}, 6}};

void emit_each_setup() {
  pinMode(LED_PIN.A, OUTPUT);
  pinMode(LED_PIN.B, OUTPUT);
  pinMode(LED_PIN.C, OUTPUT);
  pinMode(LED_PIN.D, OUTPUT);
  pinMode(LED_PIN.E, OUTPUT);
  pinMode(LED_PIN.F, OUTPUT);
  pinMode(LED_PIN.G, OUTPUT);
}

void _emit_s_led(uint8_t index, uint8_t mode) {
  switch (index) {
    case 0:
      digitalWrite(LED_PIN.A, mode);
      break;

    case 1:
      digitalWrite(LED_PIN.B, mode);
      break;

    case 2:
      digitalWrite(LED_PIN.C, mode);
      break;

    case 3:
      digitalWrite(LED_PIN.D, mode);
      break;

    case 4:
      digitalWrite(LED_PIN.E, mode);
      break;

    case 5:
      digitalWrite(LED_PIN.F, mode);
      break;

    case 6:
      digitalWrite(LED_PIN.G, mode);
      break;

    default:
      break;
  }
}

void _emit_arr_led(const uint8_t pins[7], uint8_t count, uint8_t state) {
  for (uint8_t i = 0; i < count; i++) {
    _emit_s_led(pins[i], state);
  }
}

void _set_all_led_data(uint8_t value) {
  uint8_t arr[7] = {0, 1, 2, 3, 4, 5, 6};
  _emit_arr_led(arr, 7, value);
}

void emit_num(uint8_t seg, uint8_t num) {
  uint8_t _H = H;
  uint8_t _M = L;

  pinMode(SOURCE_LED.C, INPUT);
  pinMode(SOURCE_LED.D, INPUT);

  if (seg == 0) {
    pinMode(SOURCE_LED.A, OUTPUT);
    pinMode(SOURCE_LED.B, INPUT);
    delay(1);
    digitalWrite(SOURCE_LED.A, _M);

  } else if (seg == 2) {
    pinMode(SOURCE_LED.A, OUTPUT);
    pinMode(SOURCE_LED.B, INPUT);
    delay(1);

    digitalWrite(SOURCE_LED.A, _H);

    _H = L;
    _M = H;
  } else if (seg == 1) {
    pinMode(SOURCE_LED.A, INPUT);
    pinMode(SOURCE_LED.B, OUTPUT);
    delay(1);

    digitalWrite(SOURCE_LED.B, _M);

  } else if (seg == 3) {
    pinMode(SOURCE_LED.A, INPUT);
    pinMode(SOURCE_LED.B, OUTPUT);
    delay(1);

    digitalWrite(SOURCE_LED.B, H);

    _H = L;
    _M = H;
  }

  _set_all_led_data(_M);
  delay(1);

  if (num < 10) {
    _emit_arr_led(__emit_num_obj[num].leds, __emit_num_obj[num].led_count, _H);
  }

  delay(1);
}

void emit_led(uint8_t seg, uint8_t index) {
  pinMode(SOURCE_LED.C, INPUT);
  pinMode(SOURCE_LED.D, INPUT);

  if (seg == 0) {
    pinMode(SOURCE_LED.A, OUTPUT);
    pinMode(SOURCE_LED.B, INPUT);

    _set_all_led_data(L);

    digitalWrite(SOURCE_LED.A, L);
    _emit_s_led(index, H);

  } else if (seg == 1) {
    pinMode(SOURCE_LED.A, OUTPUT);
    pinMode(SOURCE_LED.B, INPUT);

    _set_all_led_data(H);

    digitalWrite(SOURCE_LED.A, H);
    _emit_s_led(index, L);

  } else if (seg == 2) {
    pinMode(SOURCE_LED.A, INPUT);
    pinMode(SOURCE_LED.B, OUTPUT);

    _set_all_led_data(L);

    digitalWrite(SOURCE_LED.B, L);
    _emit_s_led(index, H);

  } else if (seg == 3) {
    pinMode(SOURCE_LED.A, INPUT);
    pinMode(SOURCE_LED.B, OUTPUT);

    _set_all_led_data(H);

    digitalWrite(SOURCE_LED.B, H);
    _emit_s_led(index, L);
  }

  delay(1);
}

/**
 * index = start from sunday = 0
 */
void emit_week(uint8_t index) {
  pinMode(SOURCE_LED.A, INPUT);
  pinMode(SOURCE_LED.B, INPUT);
  pinMode(SOURCE_LED.D, INPUT);
  pinMode(SOURCE_LED.C, OUTPUT);

  digitalWrite(SOURCE_LED.C, L);

  _set_all_led_data(L);

  _emit_s_led(index, H);

  delay(1);
}

/**
 * led 0,1 is each colon
 *
 * led 2 are the am/pm indicator
 *
 * @param index
 */
void emit_util(uint8_t index, uint8_t state) {
  pinMode(SOURCE_LED.A, INPUT);
  pinMode(SOURCE_LED.B, INPUT);
  pinMode(SOURCE_LED.D, INPUT);
  pinMode(SOURCE_LED.C, OUTPUT);
  uint8_t _M = H;
  if (state == _M) {
    _M = L;
  }

  digitalWrite(SOURCE_LED.C, _M);

  _set_all_led_data(_M);
  _emit_s_led(index, state);

  delay(1);
}
