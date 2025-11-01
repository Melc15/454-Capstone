#include <RTC.h>
#include <AlarmSystem.h>

int RTC_PIN = 2;
String DOW = "Friday"; // Friday
String TIME = "03:31:58"; // arbitray time
String DATE = "10/31/2025"; // arbitray date
bool motors_1[3] = {0, 1, 1}; // random motor settings
bool motors_2[3] = {1, 0, 1};
bool motors_3[3] = {1, 1, 1};
RTC rtc(RTC_PIN, DOW, TIME, DATE);
AlarmSystem sys(rtc);
volatile byte tick = 0;

void setup() {
    Serial.begin(9600);
    rtc.begin(&tick);
    sys.AddAlarm("03:32:06", DOW, motors_1);
    sys.AddAlarm("03:32:16", DOW, motors_2);
    sys.AddAlarm("03:32:08", DOW, motors_3);
    sys.AddAlarm("12:12:12", DOW, motors_1);
    Serial.println("Alarms:");
    sys.PrintAlarms();
    Serial.println("Remove alarm 3:");
    sys.RemoveAlarm(3);
    sys.PrintAlarms();
}

void loop() {
    if (tick) {
        tick = 0;
        Serial.println("Alarm 1 is on!");
        bool* motors = sys.GetMotors();
        sys.NextAlarm();
        Serial.println("Remaining alarms:");
        sys.PrintAlarms();
        delay(10);
    };
}