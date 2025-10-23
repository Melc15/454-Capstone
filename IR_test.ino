#include <RTC.h>
#include <IRSensor.h>
#include <LinearActuator.h>

int IR_LED = 1;
int IR_PIN = 2;

IRSensor irs(IR_PIN, IR_LED);
int DELAY_MS = 100;
int PILLS = 4;
int SECONDS = 15;

void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.println("Counting...");
    Serial.println("Number counted:")
    Serial.println(irs.count_breaks(15, PILLS))
    delay(9600)
}
