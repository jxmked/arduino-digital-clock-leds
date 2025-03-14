#include <Arduino.h>

struct __emit_num_values {
  uint8_t leds[7];
  uint8_t led_count;
};

struct __emit_obj {
  uint8_t a;
  uint8_t b;
};

uint8_t pins[5] = {5, 6, 7, 8, 9};
// uint8_t OBJ[3][7][2] = {
//     {{3, 4}, {3, 5}},
//     {{4, 2}, {4, 5}, {5, 4}, {4, 3}, {5, 3}, {5, 2}, {3, 2}},

//     {{6, 3}, {5, 6}, {6, 5}, {4, 6}, {6, 4}, {3, 6}, {6, 2}}};

// 3 5 6 9 10
// 5 6 7 8 9

// HIGH-LOW
uint8_t OBJ[3][7][2] = {
    {{6, 7}, {6, 8}},
    {{7, 5}, {7, 8}, {8, 7}, {7, 6}, {8, 6}, {8, 5}, {6, 5}},

    {{9, 6}, {8, 9}, {9, 8}, {7, 9}, {9, 7}, {6, 9}, {9, 5}}};

// 75 42 21
// 78 45 22
// 87 54 23
// 76 43 24
// 86 53 25
// 85 52 26
// 65 32 27

__emit_num_values num_combo[10] = {
    {{0, 1, 2, 3, 4, 5}, 6}, {{2, 3}, 2},       {{1, 2, 4, 5, 6}, 5},
    {{1, 2, 3, 4, 6}, 5},    {{0, 2, 3, 6}, 4}, {{0, 1, 3, 4, 6}, 5},
    {{0, 1, 3, 4, 5, 6}, 6}, {{1, 2, 3}, 3},    {{0, 1, 2, 3, 4, 5, 6}, 7},
    {{0, 1, 2, 3, 4, 6}, 6}};

void combo(uint8_t a, uint8_t b) {
  for (int i = 0; i < 5; i++) {
    uint8_t p = pins[i];

    if (p == a || p == b) {
      pinMode(p, OUTPUT);
    } else {
      pinMode(p, INPUT);
    }
  }

  delay(100);

  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
}

void reset_leds() {
  for (int i = 0; i < 5; i++) {
    pinMode(pins[i], INPUT);
  }
}

void emit_led(uint8_t seg, uint8_t index) {
  seg = seg % 3;

  if (seg == 0) {
    index = index % 2;
  }

  uint8_t x = OBJ[seg][index][0];
  uint8_t y = OBJ[seg][index][1];

  reset_leds();
  combo(x, y);
}

bool within_array(uint8_t arr[7], uint8_t target) {
  for (int i = 0; i < 7; i++) {
    if (arr[i] == target) return true;
  }

  return false;
}

void emit_num(uint8_t seg, uint8_t num) {
  if (seg == 0) {
    if (!num == 1) return;
  }

  __emit_num_values res_num = num_combo[num];

  // int ah = 0;
  // int al = 0;
  // int ahb = 0;
  // int alb = 0;
  // uint8_t all_high[7];
  // uint8_t all_low[7];

  // uint8_t all_high_b[7];
  // uint8_t all_low_b[7];

  for (int i = 0; i < res_num.led_count; i++) {
    uint8_t x = OBJ[seg][res_num.leds[i]][0];
    uint8_t y = OBJ[seg][res_num.leds[i]][1];

    // if (!within_array(all_high, x)) {
    //   all_high[ah] = x;
    //   ah++;
    // }

    // if (!within_array(all_low, y)) {
    //   all_low[al] = y;
    //   al++;
    // }

    // reset_leds();
    // delay(5);
    combo(x, y);
    delay(4);
  }
}

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    /* code */
  }
}

void loop() {
  emit_num(2, 8);
  delay(10);
};