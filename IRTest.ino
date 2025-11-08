#include <RTC.h>
#include <IRSensor.h>
#include <LinearActuator.h>

int IR_PIN = 2;

IRSensor irs(IR_PIN);
int DELAY_MS = 100;
int PILLS = 4;
int SECONDS = 15;

void setup() {
    Serial.begin(115200);
}

void loop() {
    Serial.println("Counting...");
    Serial.println("Number counted:")
    Serial.println(irs.count_breaks(SECONDS, PILLS, DELAY_MS))
    delay(9600)
}
