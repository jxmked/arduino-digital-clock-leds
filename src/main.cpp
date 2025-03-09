#include <Arduino.h>
#include "LED_PIN.h"

#define HIGH_STATE 1

void _emit(int a, int b, int c, int d, int e, int f, int g);
void emit_num(int sector, int num);
void emit_hour(int num);
void emit_minute(int num);

bool flag = false;

int last_time = 0;
int hour = 3;
int minute = 34;

int switch_mh = 0;

void setup()
{
  pinMode(LED_PIN.A, OUTPUT);
  pinMode(LED_PIN.B, OUTPUT);
  pinMode(LED_PIN.C, OUTPUT);
  pinMode(LED_PIN.D, OUTPUT);
  pinMode(LED_PIN.E, OUTPUT);
  pinMode(LED_PIN.F, OUTPUT);
  pinMode(LED_PIN.G, OUTPUT);

  pinMode(SOURCE_LED.A, OUTPUT);
  pinMode(SOURCE_LED.B, OUTPUT);

  digitalWrite(SOURCE_LED.A, LOW);
  digitalWrite(SOURCE_LED.B, LOW);

  digitalWrite(LED_PIN.A, HIGH);
  digitalWrite(LED_PIN.B, HIGH);
  digitalWrite(LED_PIN.C, HIGH);
  digitalWrite(LED_PIN.D, HIGH);
  digitalWrite(LED_PIN.E, HIGH);
  digitalWrite(LED_PIN.F, HIGH);
  digitalWrite(LED_PIN.G, HIGH);
}

void loop()
{

  if (millis() - last_time > 1000)
  {
    last_time = millis();
    minute++;

    if (minute == 60)
    {
      minute = 0;
      hour++;
    }

    if (hour == 24)
    {
      hour = 0;
    }
  }

  emit_minute(minute);

  // if (flag)
  // {
  // }
  // else
  // {
  //   emit_hour(hour);
  // }

  if (millis() - switch_mh >= 2000)
  {
    switch_mh = millis();
    flag = !flag;
  }

  delay(10);
}

void emit_hour(int num)
{
  // digitalWrite(LED_MH, HIGH);

  emit_num(0, num / 10);
  delay(10);
  emit_num(1, num % 10);
  delay(10);
}

void emit_minute(int num)
{
  // digitalWrite(LED_MH, LOW);

  emit_num(0, num % 10);
  // delay(10);
  // emit_num(1, num % 10);
  // delay(10);
}

int flip(int a)
{

  return a == 0 ? 1 : 0;
}

/**
 * @brief enter a number between 0 - 9
 *
 * @param num
 */
void emit_num(int sector, int num)
{
  if (sector == 0)
  {
    digitalWrite(SOURCE_LED.A, LOW);
    digitalWrite(SOURCE_LED.B, HIGH);
  }
  else if (sector == 1)
  {
    digitalWrite(SOURCE_LED.A, HIGH);
    digitalWrite(SOURCE_LED.B, LOW);
  }
  else
  {
  }

  int h = HIGH_STATE;
  int l = HIGH_STATE == 1 ? 0 : 1;

  switch (num)
  {
  case 1:
    // 1 = 2, 3
    _emit(l, l, h, h, l, l, l);
    break;

  case 2:
    // 2 = 1, 2, 4, 5, 6
    _emit(l, h, h, l, h, h, h);
    break;

  case 3:
    // 3 = 1, 2, 3, 4, 6

    _emit(l, h, h, h, h, l, h);
    break;

  case 4:
    // 4 = 0, 2, 3, 6

    _emit(h, l, h, h, l, l, h);
    break;

  case 5:
    // 5 = 0, 1, 3, 4, 6

    _emit(h, h, l, h, h, l, h);
    break;

  case 6:
    // 6 = 0, 1, 3, 4, 5, 6

    _emit(h, h, l, h, h, h, h);
    break;

  case 7:
    // 7 = 1, 2, 3
    _emit(l, h, h, h, l, l, l);
    break;

  case 8:
    // 8 = -all
    _emit(h, h, h, h, h, h, h);
    break;

  case 9:
    // 9 = 0, 1, 2, 3, 4, 6
    _emit(h, h, h, h, h, l, h);
    break;

  case 0:
    // 0 = -all, except 6
    _emit(h, h, h, h, h, h, l);
    break;

  default:
    _emit(l, l, l, l, l, l, l);
    break;
  }
}

void _emit(int a, int b, int c, int d, int e, int f, int g)
{

  digitalWrite(LED_PIN.A, a);
  digitalWrite(LED_PIN.B, b);
  digitalWrite(LED_PIN.C, c);
  digitalWrite(LED_PIN.D, d);
  digitalWrite(LED_PIN.E, e);
  digitalWrite(LED_PIN.F, f);
  digitalWrite(LED_PIN.G, g);
}