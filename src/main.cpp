#include <Arduino.h>
#include <TimeInterval.h>

#include "emit_each.h"
#include "esp_pm.h"

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
  auto current_num = (millis() / 500) % 28;

  if (current_num < 7) emit_led(0, current_num % 7);
  if (current_num >= 7 && current_num < 14) emit_led(1, current_num % 7);
  if (current_num >= 14 && current_num < 21) emit_led(2, current_num % 7);
  if (current_num >= 21 && current_num < 28) emit_led(3, current_num % 7);
}