

#include <Arduino.h>

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

void _emit_arr_led(int pins[7], int count, uint8_t state) {
  for (int i = 0; i < count; i++) {
    _emit_s_led(pins[i], state);
  }
}

void emit_num(int seg, int num) {
  uint8_t H = HIGH;
  uint8_t L = LOW;

  pinMode(SOURCE_LED.A, OUTPUT);
  pinMode(SOURCE_LED.B, OUTPUT);
  pinMode(SOURCE_LED.C, INPUT);
  pinMode(SOURCE_LED.D, INPUT);

  if (seg == 0) {
    pinMode(SOURCE_LED.B, INPUT);
    digitalWrite(SOURCE_LED.A, L);

  } else if (seg == 1) {
    pinMode(SOURCE_LED.B, INPUT);
    digitalWrite(SOURCE_LED.A, H);

    H = LOW;
    L = HIGH;
  } else if (seg == 2) {
    pinMode(SOURCE_LED.A, INPUT);
    digitalWrite(SOURCE_LED.B, L);

  } else if (seg == 3) {
    pinMode(SOURCE_LED.A, INPUT);
    digitalWrite(SOURCE_LED.B, H);

    H = LOW;
    L = HIGH;
  }

  _set_all_led_data(L);
  switch (num) {
    case 0: {
      int to_emit[7] = {0, 1, 2, 3, 4, 5};
      _emit_arr_led(to_emit, 6, H);
    } break;

    case 1: {
      int to_emit[7] = {2, 3};
      _emit_arr_led(to_emit, 2, H);
    } break;

    case 2: {
      int to_emit[7] = {1, 2, 4, 5, 6};
      _emit_arr_led(to_emit, 5, H);
    } break;

    case 3: {
      int to_emit[7] = {1, 2, 3, 4, 6};
      _emit_arr_led(to_emit, 5, H);
    } break;

    case 4: {
      int to_emit[7] = {0, 2, 3, 6};
      _emit_arr_led(to_emit, 4, H);
    } break;

    case 5: {
      int to_emit[7] = {0, 1, 3, 4, 6};
      _emit_arr_led(to_emit, 5, H);
    } break;

    case 6: {
      int to_emit[7] = {0, 1, 3, 4, 5, 6};
      _emit_arr_led(to_emit, 6, H);
    } break;

    case 7: {
      int to_emit[7] = {1, 2, 3};
      _emit_arr_led(to_emit, 3, H);
    } break;

    case 8: {
      int to_emit[7] = {
          0, 1, 2, 3, 4, 5, 6,
      };
      _emit_arr_led(to_emit, 7, H);
    } break;

    case 9: {
      int to_emit[7] = {0, 1, 2, 3, 4, 6};
      _emit_arr_led(to_emit, 6, H);
    } break;

    default:
      break;
  }
}

void emit_led(int seg, int index) {
  pinMode(SOURCE_LED.A, OUTPUT);
  pinMode(SOURCE_LED.B, OUTPUT);
  pinMode(SOURCE_LED.C, INPUT);
  pinMode(SOURCE_LED.D, INPUT);

  if (seg == 0) {
    pinMode(SOURCE_LED.B, INPUT);

    digitalWrite(SOURCE_LED.A, LOW);
    _emit_s_led(index, HIGH);

  } else if (seg == 1) {
    pinMode(SOURCE_LED.B, INPUT);

    digitalWrite(SOURCE_LED.A, HIGH);
    _emit_s_led(index, LOW);

  } else if (seg == 2) {
    pinMode(SOURCE_LED.A, INPUT);

    digitalWrite(SOURCE_LED.B, LOW);
    _emit_s_led(index, HIGH);

  } else if (seg == 3) {
    pinMode(SOURCE_LED.A, INPUT);

    digitalWrite(SOURCE_LED.B, HIGH);
    _emit_s_led(index, LOW);
  }
}

/**
 * index = start from sunday = 0
 */
void emit_week(int index) {
  pinMode(SOURCE_LED.A, INPUT);
  pinMode(SOURCE_LED.B, INPUT);
  pinMode(SOURCE_LED.D, INPUT);
  pinMode(SOURCE_LED.C, OUTPUT);

  digitalWrite(SOURCE_LED.C, LOW);

  _set_all_led_data(LOW);

  _emit_s_led(index, HIGH);
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

  digitalWrite(SOURCE_LED.D, LOW);

  _set_all_led_data(LOW);
  _emit_s_led(index, HIGH);
}
