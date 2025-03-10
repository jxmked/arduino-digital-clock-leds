#include <Arduino.h>

#include "LED_PIN.h"

void emit_each_setup() {
  pinMode(LED_PIN.A, OUTPUT);
  pinMode(LED_PIN.B, OUTPUT);
  pinMode(LED_PIN.C, OUTPUT);
  pinMode(LED_PIN.D, OUTPUT);
  pinMode(LED_PIN.F, OUTPUT);
  pinMode(LED_PIN.G, OUTPUT);
}

void _set_led_data(int value) {
  digitalWrite(LED_PIN.A, value);
  digitalWrite(LED_PIN.B, value);
  digitalWrite(LED_PIN.C, value);
  digitalWrite(LED_PIN.D, value);
  digitalWrite(LED_PIN.E, value);
  digitalWrite(LED_PIN.F, value);
  digitalWrite(LED_PIN.G, value);
}

void emit_led(int seg, int index) {
  pinMode(SOURCE_LED.A, OUTPUT);
  pinMode(SOURCE_LED.B, OUTPUT);
  pinMode(SOURCE_LED.C, INPUT);
  pinMode(SOURCE_LED.D, INPUT);

  if (seg == 0) {
    pinMode(SOURCE_LED.B, INPUT);

    digitalWrite(SOURCE_LED.A, LOW);
    digitalWrite(index, HIGH);

  } else if (seg == 1) {
    pinMode(SOURCE_LED.B, INPUT);

    digitalWrite(SOURCE_LED.A, HIGH);
    digitalWrite(index, LOW);

  } else if (seg == 2) {
    pinMode(SOURCE_LED.A, INPUT);

    digitalWrite(SOURCE_LED.B, LOW);
    digitalWrite(index, HIGH);

  } else if (seg == 3) {
    pinMode(SOURCE_LED.A, INPUT);

    digitalWrite(SOURCE_LED.B, LOW);
    digitalWrite(index, HIGH);
  }
}

/**
 * index = start from sun = 0
 *
 */
void emit_week(int index) {
  pinMode(SOURCE_LED.A, INPUT);
  pinMode(SOURCE_LED.B, INPUT);
  pinMode(SOURCE_LED.D, INPUT);
  pinMode(SOURCE_LED.C, LOW);

  _set_led_data(LOW);

  switch (index) {
    case 0:
      digitalWrite(LED_PIN.A, HIGH);
      break;

    case 1:
      digitalWrite(LED_PIN.B, HIGH);
      break;

    case 2:
      digitalWrite(LED_PIN.C, HIGH);
      break;

    case 3:
      digitalWrite(LED_PIN.D, HIGH);
      break;

    case 4:
      digitalWrite(LED_PIN.E, HIGH);
      break;

    case 5:
      digitalWrite(LED_PIN.F, HIGH);
      break;

    case 6:
      digitalWrite(LED_PIN.G, HIGH);
      break;

    default:
      break;
  }
}
