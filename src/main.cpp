#include <Arduino.h>
#include <TimeInterval.h>

#include "connect.h"
#include "emit_each.h"
#include "esp_pm.h"

struct _TIME_t {
  uint8_t hour;
  uint8_t minute;
};

_TIME_t cur_time;

TimeInterval timer = TimeInterval(500, 0, true);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  connect_setup();

  // Use build time as initial time
  const char* timeStr = __TIME__;
  uint8_t build_hour = (timeStr[0] - '0') * 10 + (timeStr[1] - '0');
  uint8_t build_minute =
      ((timeStr[3] - '0') * 10 + (timeStr[4] - '0') + 1) % 60;

  cur_time.hour = build_hour;
  cur_time.minute = build_minute;

  emit_each_setup();
}

void loop() {
  emit_refresh();

  connect_loop();

  auto current_num = (millis() / 1000);

  auto minute = current_num / 100;
  emit_num(0, current_num);
  emit_num(1, current_num / 10);

  // delay(delays);

  emit_num(2, minute);
  emit_num(3, minute / 10);
}