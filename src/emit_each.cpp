#include <Arduino.h>
#include <DEFINITION.h>

#include "LED_PIN.h"

void emit_each_setup() {
  pinMode(LED_PIN.A, OUTPUT);
  pinMode(LED_PIN.B, OUTPUT);
  pinMode(LED_PIN.C, OUTPUT);
  pinMode(LED_PIN.D, OUTPUT);
  pinMode(LED_PIN.E, OUTPUT);
  pinMode(LED_PIN.F, OUTPUT);
  pinMode(LED_PIN.G, OUTPUT);
}

void _set_all_led_data(int value) {
  digitalWrite(LED_PIN.A, value);
  digitalWrite(LED_PIN.B, value);
  digitalWrite(LED_PIN.C, value);
  digitalWrite(LED_PIN.D, value);
  digitalWrite(LED_PIN.E, value);
  digitalWrite(LED_PIN.F, value);
  digitalWrite(LED_PIN.G, value);
}

void _emit_s_led(int index, int mode) {
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

void _emit_arr_led(const int pins[7], int count, uint8_t state) {
  for (int i = 0; i < count; i++) {
    _emit_s_led(pins[i], state);
  }
}

void emit_num(int seg, int num) {
  uint8_t _H = H;
  uint8_t _M = L;

  pinMode(SOURCE_LED.C, INPUT);
  pinMode(SOURCE_LED.D, INPUT);

  if (seg == 0) {
    pinMode(SOURCE_LED.A, OUTPUT);
    pinMode(SOURCE_LED.B, INPUT);
    digitalWrite(SOURCE_LED.A, _M);

  } else if (seg == 1) {
    pinMode(SOURCE_LED.A, OUTPUT);
    pinMode(SOURCE_LED.B, INPUT);
    digitalWrite(SOURCE_LED.A, _H);

    _H = L;
    _M = H;
  } else if (seg == 2) {
    pinMode(SOURCE_LED.A, INPUT);
    pinMode(SOURCE_LED.B, OUTPUT);

    digitalWrite(SOURCE_LED.B, _M);

  } else if (seg == 3) {
    pinMode(SOURCE_LED.A, INPUT);
    pinMode(SOURCE_LED.B, OUTPUT);

    digitalWrite(SOURCE_LED.B, H);

    _H = L;
    _M = H;
  }

  _set_all_led_data(_M);
  switch (num) {
    case 0: {
      const int to_emit[7] = {0, 1, 2, 3, 4, 5};
      _emit_arr_led(to_emit, 6, _H);
    } break;

    case 1: {
      const int to_emit[7] = {2, 3};
      _emit_arr_led(to_emit, 2, _H);
    } break;

    case 2: {
      const int to_emit[7] = {1, 2, 4, 5, 6};
      _emit_arr_led(to_emit, 5, _H);
    } break;

    case 3: {
      const int to_emit[7] = {1, 2, 3, 4, 6};
      _emit_arr_led(to_emit, 5, _H);
    } break;

    case 4: {
      const int to_emit[7] = {0, 2, 3, 6};
      _emit_arr_led(to_emit, 4, _H);
    } break;

    case 5: {
      const int to_emit[7] = {0, 1, 3, 4, 6};
      _emit_arr_led(to_emit, 5, _H);
    } break;

    case 6: {
      const int to_emit[7] = {0, 1, 3, 4, 5, 6};
      _emit_arr_led(to_emit, 6, _H);
    } break;

    case 7: {
      const int to_emit[7] = {1, 2, 3};
      _emit_arr_led(to_emit, 3, _H);
    } break;

    case 8: {
      const int to_emit[7] = {
          0, 1, 2, 3, 4, 5, 6,
      };
      _emit_arr_led(to_emit, 7, _H);
    } break;

    case 9: {
      const int to_emit[7] = {0, 1, 2, 3, 4, 6};
      _emit_arr_led(to_emit, 6, _H);
    } break;

    default:
      break;
  }

  delay(1);
}

void emit_led(int seg, int index) {
  pinMode(SOURCE_LED.C, INPUT);
  pinMode(SOURCE_LED.D, INPUT);

  if (seg == 0) {
    pinMode(SOURCE_LED.A, OUTPUT);
    pinMode(SOURCE_LED.B, INPUT);

    digitalWrite(SOURCE_LED.A, L);
    _emit_s_led(index, H);

  } else if (seg == 1) {
    pinMode(SOURCE_LED.A, OUTPUT);
    pinMode(SOURCE_LED.B, INPUT);

    digitalWrite(SOURCE_LED.A, H);
    _emit_s_led(index, L);

  } else if (seg == 2) {
    pinMode(SOURCE_LED.A, INPUT);
    pinMode(SOURCE_LED.B, OUTPUT);

    digitalWrite(SOURCE_LED.B, L);
    _emit_s_led(index, H);

  } else if (seg == 3) {
    pinMode(SOURCE_LED.A, INPUT);
    pinMode(SOURCE_LED.B, OUTPUT);

    digitalWrite(SOURCE_LED.B, H);
    _emit_s_led(index, L);
  }

  delay(1);
}

/**
 * index = start from sunday = 0
 */
void emit_week(int index) {
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
void emit_util(int index) {
  pinMode(SOURCE_LED.A, INPUT);
  pinMode(SOURCE_LED.B, INPUT);
  pinMode(SOURCE_LED.C, INPUT);
  pinMode(SOURCE_LED.D, OUTPUT);

  digitalWrite(SOURCE_LED.D, L);

  _set_all_led_data(L);
  _emit_s_led(index, H);

  delay(1);
}
