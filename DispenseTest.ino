#include <RTC.h>
#include <Dispenser.h>

int IR_PIN = 3;
IRSensor irs(IR_PIN);

int RTC_PIN = 2;
String DOW = "Friday"; // Friday
String TIME = "03:31:58"; // arbitray time
String DATE = "10/31/2025"; // arbitray date
RTC rtc(RTC_PIN, DOW, TIME, DATE);

int DOUT_pin = 4;
int SCK_pin = 5;
LoadCell scale(DOUT_pin, SCK_pin);

Dispenser sys(4, rtc, irs, scale);
volatile byte tick = 0;

int pills_1[3] = {1, 2, 3};
int pills_2[3] = {2, 1, 0};

void setup() {
    Serial.begin(115200);
    sys.begin(&tick);
    sys.AddAlarm("03:33:06", DOW, pills_1, 1);
    sys.AddAlarm("03:34:16", DOW, pills_2, 1);
    sys.AddAlarm("03:35:08", DOW, pills_1, 1);
    delay(10);
}

void loop() {
    if (tick) {
        tick = 0;
        Serial.println("Alarm is on!");
        sys.Dispense();
        while(!tick){
            delay(10);
        }
        tick = 0;
        while(!tick){
            delay(10);
        }
        sys.pills_taken();
        sys.NextAlarm();
        Serial.println("Remaining alarms:");
        sys.PrintAlarms();
        delay(10);
    };
}
