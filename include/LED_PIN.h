#ifndef LED_PIN_h
#define LED_PIN_h

#include <Arduino.h>

struct LED_PIN_DATA_T {
  int A = A4;
  int B = A5;
  int C = 6;
  int D = 7;
  int E = 8;
  int F = 9;
  int G = 10;
};

struct SOURCE_LED_T {
  int A = A0;
  int B = A1;
  int C = A2;
  int D = A3;
};

const SOURCE_LED_T SOURCE_LED;
const LED_PIN_DATA_T LED_PIN;

#endif