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
  emit_refresh();

  auto current_num = (millis() / 1000);
  auto delays = 1;
  // static uint8_t cur;
  // if (Serial.available() > 0) {
  //   int input = Serial.parseInt();  // Read the integer input

  //   // Validate if it's within 8-bit range (0-255)
  //   if (input >= 0 && input <= 255) {
  //     uint8_t receivedValue = (uint8_t)input;  // Convert to uint8_t

  //     Serial.print("Received 8-bit value: ");
  //     Serial.println(receivedValue);
  //     cur = receivedValue;
  //   } else {
  //     Serial.println("Invalid input! Please enter a number between 0 and 255.");
  //   }

  //   // Flush buffer
  //   while (Serial.available()) Serial.read();
  // }

  // if (timer.marked(500)) {
  //   emit_util(0, L);
  //   emit_util(1, L);
  // } else {
  //   emit_util(0, H);
  //   emit_util(1, H);
  // }

  auto minute = current_num / 100;
  emit_num(2, current_num);
  // emit_num(1, (current_num / 10) % 10);

  // delay(delays);

  // emit_num(2, minute % 10);
  // emit_num(3, (minute / 10) % 10);
}