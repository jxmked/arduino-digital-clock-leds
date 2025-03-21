#include "emit_each.h"

#include <Arduino.h>

#include "LED_PIN.h"
#include "TimeInterval.h"
#define COMMON_ANODE 0x0
#define COMMON_CATHODE 0x80

#define LED_INTERVAL 1000

TimeInterval colon_ival = TimeInterval(500, 0, true);

//  BBB
// A   C
// A   C
//  GGG
// F   D
// F   D
//  EEE

// - = set if common anode or common cathode
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
    0b01100000,  // :  - 10
};

const uint8_t segment_map[7] = {LED_PIN.A, LED_PIN.B, LED_PIN.C, LED_PIN.D,
                                LED_PIN.E, LED_PIN.F, LED_PIN.G};

const uint8_t digit_map[6] = {SOURCE_LED.A, SOURCE_LED.B, SOURCE_LED.A,
                              SOURCE_LED.B, SOURCE_LED.C, SOURCE_LED.D};

// const uint8_t digit_map[5] = {SOURCE_LED.A, SOURCE_LED.B, SOURCE_LED.C,
//                               SOURCE_LED.D};

const uint8_t segment_count = 7;
const uint8_t digit_count = 6;
const uint8_t COLON_IDX = 4;

uint8_t digit_number[digit_count];
uint8_t digit_idx = 0;

unsigned long last_led_interval = 0;

void emit_each_setup() {
  for (int i = 0; i < segment_count; i++) {
    pinMode(segment_map[i], OUTPUT);
  }
}

void emit_led_digit(uint8_t digit) {
  for (int i = 0; i < digit_count; i++) {
    pinMode(digit_map[i], INPUT);
  }

  uint8_t code = digit_number[digit];

  if (code == COMMON_ANODE || code == COMMON_CATHODE) {
    return;
  }

  uint8_t cur_digit_pin = digit_map[digit];
  uint8_t s_pos = HIGH;
  uint8_t s_neg = LOW;

  if (!(code & COMMON_CATHODE)) {
    s_pos = LOW;
    s_neg = HIGH;
  }

  for (uint8_t i = 0; i < 7; i++) {
    digitalWrite(segment_map[i], s_pos);
  }

  for (uint8_t i = 0; i < 7; i++) {
    bool res = (code & (1 << (6 - i))) ? s_neg : s_pos;
    digitalWrite(segment_map[i], res);
  }

  delayMicroseconds(100);

  /**
   * idk but this works.
   * it completely eliminate some
   * led that arent assigned to lit
   *
   */
  digitalWrite(cur_digit_pin, s_pos);
  digitalWrite(cur_digit_pin, s_pos);
  digitalWrite(cur_digit_pin, s_pos);

  pinMode(cur_digit_pin, OUTPUT);
  digitalWrite(cur_digit_pin, s_pos);
}

void emit_num(uint8_t digit, uint8_t num) {
  digit = digit % digit_count;
  digit_number[digit] = digit_codes[num % 10];

  // Set which is common is cathode or anode
  if (digit == 0 || digit == 1) digit_number[digit] |= COMMON_ANODE;
  if (digit == 2 || digit == 3) digit_number[digit] |= COMMON_CATHODE;
}

void emit_refresh() {
  unsigned long ms = micros();

  if (ms - last_led_interval > LED_INTERVAL) {
    emit_led_digit(digit_idx);

    digit_idx++;
    if (digit_idx >= digit_count) digit_idx = 0;

    last_led_interval = micros();
  }
}

void emit_show_colon(bool keep_off) {
  if (colon_ival.marked(500) && !keep_off) {
    // pin A2
    digit_number[COLON_IDX] = digit_codes[10];
    digit_number[COLON_IDX] |= COMMON_CATHODE;
  } else {
    digit_number[COLON_IDX] = 0;
  }
}

void emit_clear_digit(uint8_t digit) { digit_number[digit] = 0; }

void emit_display_char(uint8_t digit, uint8_t ui_8) {
  digit = digit % digit_count;
  digit_number[digit] = ui_8;

  // Set which is common is cathode or anode
  if (digit == 0 || digit == 1) digit_number[digit] |= COMMON_ANODE;
  if (digit == 2 || digit == 3) digit_number[digit] |= COMMON_CATHODE;
}
