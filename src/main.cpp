#include <Arduino.h>
#include <TimeInterval.h>

#include "emit_each.h"

TimeInterval counter = TimeInterval(1000, 0, true);

int i = 0;

void setup() { emit_each_setup(); }

void loop() {
  if (counter.marked()) {
    i++;

    if (i >= 10) {
      i = 0;
    }
  }

  emit_num(0, i);
}