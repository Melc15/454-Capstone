#include <Dispenser.h>

int IR_PIN = 3;
IRSensor irs(IR_PIN);

int RTC_PIN = 2;
String DOW = "Friday"; // Friday
String TIME = "12:35:59"; // arbitray time
String DATE = "11/13/2025"; // arbitray date
RTC rtc(RTC_PIN, DOW, TIME, DATE);

int DOUT_pin = 4;
int SCK_pin = 5;
LoadCell scale(DOUT_pin, SCK_pin);

Dispenser sys(6, rtc, irs, scale);
volatile byte tick = 0;

int pills[6] = {1, 1, 1, 1, 1, 1};

void setup() {
    Serial.begin(115200);
    sys.begin(&tick);
    sys.AddAlarm("12:37:05", DOW, pills, 1);
    delay(10);
}

void loop() {
    if (tick) {
        tick = 0;
        Serial.println("Alarm is on!");
        sys.Dispense(1);
        while(!tick){
            delay(10);
        }
        tick = 0;
        while(!tick){
            delay(10);
        }
        sys.pills_taken();
        delay(10);
    };
}
