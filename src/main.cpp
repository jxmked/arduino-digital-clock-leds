#include <Arduino.h>
#include <TimeInterval.h>

#include "ENV.h"
#include "TYPES.h"
#include "connect.h"
#include "emit_each.h"
#include "esp_pm.h"

#define MILITARY_TIME 0

_TIME_t cur_time;

WIFI_INTERVAL wifi_update_conf;
SWITCH_TIME_DATE switching_time;

// klik-klok hehe - 1 minute
// This is not precise since its a hardware problem
// That's why we need to update the date/time every
// Specific interval
TimeInterval time_tick = TimeInterval(60000, 0, true);

// Update date/time interval
TimeInterval update_time_ival = TimeInterval(wifi_update_conf.TURN_ON, 0, true);

// Switch display between date and time
TimeInterval switching_ival = TimeInterval(switching_time.DATE_ON, 0, true);

// Prevent showing date when its not set/updated
bool is_date_set = false;
unsigned long last_update = 0;

static void display_date(void);
static void display_time(void);
static bool update_time(void);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  connect_setup();
  emit_each_setup();

  // Use build time as initial time
  const char* timeStr = __TIME__;
  uint8_t build_hour = (timeStr[0] - '0') * 10 + (timeStr[1] - '0');
  uint8_t build_minute =
      ((timeStr[3] - '0') * 10 + (timeStr[4] - '0') + 1) % 60;

  cur_time.hour = build_hour;
  cur_time.minute = build_minute;

  // INITIAL UPDATE
  unsigned long last_check = 0;
  int anim_ival = 720;
  while (true) {
    auto ms = millis();

    emit_refresh();

    emit_clear_digit(0);
    emit_clear_digit(1);
    emit_clear_digit(2);
    emit_clear_digit(3);

    emit_display_char((ms / anim_ival) % 4,
                      0x40 >> (6 - ((ms / (anim_ival / 7)) % 7)));

    if (millis() - last_check >= 1000) {
      if (update_time()) break;
      last_check = millis();
    }
  }
}

void loop() {
  emit_refresh();

  // update if the day just passed
  if (cur_time.hour == 0 && cur_time.minute <= 3) {
    is_date_set = false;
  }

  // Keep 30 mins difference from last update
  if (!is_date_set && (millis() - last_update >= (60 * 30 * 1000))) {
    update_time();
  }

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

  // prevent showing the date when its not being updated
  if (!is_date_set || switching_ival.marked(switching_time.TIME_ON)) {
    display_date();
  } else {
    display_time();
  }
}

static bool update_time(void) {
  UPDATE_TIME_CONST status = connect_loop();

  if (status == UPDATE_TIME_CONST::FAIL) {
    update_time_ival.reset();

    is_date_set = false;

    last_update = millis();

    return true;
  } else if (status == UPDATE_TIME_CONST::OK) {
    connect_update_time(&cur_time);
    update_time_ival.reset();

    is_date_set = true;

    last_update = millis();

    return true;
  }

  return false;
}

static void display_time(void) {
  uint8_t minute = cur_time.minute;
  uint8_t hour = cur_time.hour;

  // Only affect the time when its standby
  if (MILITARY_TIME == 0) {
    if (hour == 0) {
      hour = 12;
    } else {
      hour = hour > 12 ? hour - 12 : hour;
    }
  }

  emit_show_colon();

  emit_num(0, minute);
  emit_num(1, minute / 10);

  emit_num(2, hour);

  emit_num(3, hour / 10);
  if (hour < 10) emit_clear_digit(3);
}

static void display_date(void) {
  emit_show_colon(true);

  emit_num(0, cur_time.month);
  emit_num(1, cur_time.month / 10);

  // Turn off other zero at the front
  if (cur_time.month < 10) emit_clear_digit(1);

  emit_num(2, cur_time.day);
  emit_num(3, cur_time.day / 10);

  // Turn off other zero at the front
  if (cur_time.day < 10) emit_clear_digit(3);
}