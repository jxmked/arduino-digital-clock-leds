#ifndef LED_PIN_h
#define LED_PIN_h

#include <Arduino.h>

typedef struct {
  uint8_t A = A4;
  uint8_t B = A5;
  uint8_t C = 6;
  uint8_t D = 7;
  uint8_t E = 8;
  uint8_t F = 9;
  uint8_t G = 10;
} LED_PIN_DATA_T;

typedef struct {
  uint8_t A = A0;
  uint8_t B = A1;
  uint8_t C = A2;
  uint8_t D = A3;
} SOURCE_LED_T;

const SOURCE_LED_T SOURCE_LED;
const LED_PIN_DATA_T LED_PIN;

#endif