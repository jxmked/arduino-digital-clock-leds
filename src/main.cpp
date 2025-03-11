#include <Arduino.h>
#include <TimeInterval.h>

#include "emit_each.h"
#include "esp_pm.h"

void setup() {
  esp_pm_config_esp32c3_t pm_config = {.max_freq_mhz = 80,
                                       .light_sleep_enable = true};

  esp_pm_configure(&pm_config);

  emit_each_setup();
}

void loop() {
  emit_num(0, (millis() / 500) % 10);
  emit_num(1, (millis() / 500) % 10);
  emit_num(2, (millis() / 500) % 10);
  emit_num(3, (millis() / 500) % 10);
}