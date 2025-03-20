#include <Arduino.h>
#include <TimeInterval.h>

#include "ENV.h"
#include "TYPES.h"
#include "connect.h"
#include "emit_each.h"
#include "esp_pm.h"

WIFI_INTERVAL wifi_update_conf;

// 1 minute
TimeInterval time_tick = TimeInterval(60000, 0, true);

TimeInterval update_time_ival = TimeInterval(wifi_update_conf.TURN_ON, 0, true);

_TIME_t cur_time;

bool update_time() {
  UPDATE_TIME_CONST status = connect_loop();

  if (status == UPDATE_TIME_CONST::FAIL) {
    update_time_ival.reset();
    return true;
  } else if (status == UPDATE_TIME_CONST::OK) {
    connect_update_time(&cur_time);
    update_time_ival.reset();
    return true;
  }

  return false;
}

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

  int attempt = 0;
  while (true) {
    if (update_time() || attempt >= 1000) {
      break;
    }
    attempt++;
  }

  emit_each_setup();
}

void loop() {
  emit_refresh();

  if (update_time_ival.marked(wifi_update_conf.KEEP_ON)) {
    update_time();
  }

  if (time_tick.marked()) {
    cur_time.minute++;

    if (cur_time.minute >= 60) {
      cur_time.minute = 0;

      cur_time.hour++;

      if (cur_time.hour >= 24) {
        cur_time.hour = 0;
      }
    }
  }

  emit_show_colon();

  emit_num(0, cur_time.minute);
  emit_num(1, cur_time.minute / 10);

  emit_num(2, cur_time.hour);
  emit_num(3, cur_time.hour / 10);
}