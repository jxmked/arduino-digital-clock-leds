#ifndef LED_PIN_h
#define LED_PIN_h

#include <Arduino.h>

struct LED_PIN_DATA_T
{
  int A = A3;
  int B = A4;
  int C = A5;
  int D = 6;
  int E = 7;
  int F = 10;
  int G = 9;
};

struct SOURCE_LED_T
{
  int A = A0;
  int B = A1;
};

const SOURCE_LED_T SOURCE_LED;
const LED_PIN_DATA_T LED_PIN;

#endif