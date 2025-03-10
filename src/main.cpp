#include <Arduino.h>
#include <TimeInterval.h>

void setup() {
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);

    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
}

void loop() {
    digitalWrite(5, !digitalRead(5));
    digitalWrite(6, !digitalRead(6));

    delay(500);
}