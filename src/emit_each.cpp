#include "emit_each.h"

#include <Arduino.h>
#include <DEFINITION.h>

#include "LED_PIN.h"

#define COMMON_ANODE 0x0
#define COMMON_CATHODE 0x80

#define LED_INTERVAL 2000

//  BBB
// A   C
// A   C
//  GGG
// F   D
// F   D
//  EEE

// -ABCDEFG
uint8_t digit_codes[] = {
    0b01111110,  // 0
    0b00011000,  // 1
    0b00110111,  // 2
    0b00111101,  // 3
    0b01011001,  // 4
    0b01101101,  // 5
    0b01101111,  // 6
    0b00111000,  // 7
    0b01111111,  // 8
    0b01111101,  // 9

    // DONT USE FOR NUMBERS
    // COLON
    0b01100000,  // :
};

const uint8_t segment_map[7] = {LED_PIN.A, LED_PIN.B, LED_PIN.C, LED_PIN.D,
                                LED_PIN.E, LED_PIN.F, LED_PIN.G};

const uint8_t digit_map[5] = {SOURCE_LED.A, SOURCE_LED.B, SOURCE_LED.C,
                              SOURCE_LED.D};

const uint8_t digit_count = 5;
uint8_t digit_number[5];
uint8_t digit_idx = 0;

unsigned long last_led_interval = 0;

void emit_each_setup() {
  pinMode(LED_PIN.A, OUTPUT);
  pinMode(LED_PIN.B, OUTPUT);
  pinMode(LED_PIN.C, OUTPUT);
  pinMode(LED_PIN.D, OUTPUT);
  pinMode(LED_PIN.E, OUTPUT);
  pinMode(LED_PIN.F, OUTPUT);
  pinMode(LED_PIN.G, OUTPUT);
}

void emit_led_digit(uint8_t digit) {
  for (int i = 0; i < digit_count; i++) {
    pinMode(digit_map[i], INPUT);
    // analogReadMilliVolts(digit_map[i]);
  }

  // delayMicroseconds(10);

  uint8_t code = digit_number[digit];

  if (code == COMMON_ANODE || code == COMMON_CATHODE) {
    return;
  }

  uint8_t cur_digit_pin;

  if (digit == 0 || digit == 2) {
    cur_digit_pin = digit_map[0];
  } else if (digit == 1 || digit == 3) {
    cur_digit_pin = digit_map[1];
  }

  pinMode(cur_digit_pin, OUTPUT);

  if (code & COMMON_CATHODE) {
    for (uint8_t i = 0; i < 7; i++) {
      bool res = (code & (1 << (6 - i))) ? LOW : HIGH;
      digitalWrite(segment_map[i], res);
    }

    delayMicroseconds(20);

    digitalWrite(cur_digit_pin, HIGH);
  } else {
    for (uint8_t i = 0; i < 7; i++) {
      bool res = (code & (1 << (6 - i))) ? HIGH : LOW;
      digitalWrite(segment_map[i], res);
    }

    delayMicroseconds(20);
    digitalWrite(cur_digit_pin, LOW);
  }
}

void emit_num(uint8_t seg, uint8_t num) {
  num = num % 10;
  digit_number[seg] = digit_codes[num];

  // Set which is common is cathode or anode
  if (seg == 0 || seg == 1) digit_number[seg] |= COMMON_ANODE;
  if (seg == 2 || seg == 3) digit_number[seg] |= COMMON_CATHODE;

  // for (int i = 0; i < 8; i++) Serial.print(bitRead(digit_number[seg], i));
  // Serial.println();
  // Serial.println(num);
}

void emit_refresh() {
  unsigned long ms = micros();

  if (ms - last_led_interval > LED_INTERVAL) {
    emit_led_digit(digit_idx);
    // delayMicroseconds(2000);

    digit_idx++;

    if (digit_idx >= digit_count) {
      digit_idx = 0;
    }

    // delayMicroseconds(LED_INTERVAL);

    last_led_interval = micros();
  }
}