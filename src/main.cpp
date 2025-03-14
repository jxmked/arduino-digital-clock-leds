#include <Arduino.h>
#include <DEFINITION.h>
#include <TimeInterval.h>

#include "emit_each.h"
#include "esp_pm.h"

TimeInterval timer = TimeInterval(500, 0, true);

esp_pm_config_esp32c3_t pm_config = {

    .light_sleep_enable = true};

void setup() {
  esp_err_t res = esp_pm_configure(&pm_config);

  setCpuFrequencyMhz(10);

  Serial.begin(9600);

  delay(2000);
  // while (!Serial) {
  //   Serial.print("Initializing serial...");
  // }

  if (res == ESP_OK) {
    Serial.println("ESP Configured");
  } else if (res == ESP_ERR_INVALID_ARG) {
    Serial.println("Invalid Argument");
  } else if (res == ESP_ERR_NOT_SUPPORTED) {
    Serial.println("Err not supported");
  } else {
    Serial.print("Error not recognized");
    Serial.print(res);
  }

  emit_each_setup();
}

void loop() {
  auto current_num = (millis() / 250);
  auto delays = 1;

  // if (timer.marked(500)) {
  //   emit_util(0, L);
  //   emit_util(1, L);
  // } else {
  //   emit_util(0, H);
  //   emit_util(1, H);
  // }

  auto minute = current_num / 100;

  delay(delays);

  emit_num(0, current_num % 10);
  delay(delays);
  emit_num(2, minute % 10);
  delay(delays);
  emit_num(1, (current_num / 10) % 10);
  delay(delays);
  emit_num(3, (minute / 10) % 10);
  delay(delays);
}