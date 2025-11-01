#include <RTC.h>

int RTC_PIN = 2;
byte DOW = 5; // Friday
String TIME = "03:33:50"; // arbitray time
String DATE = "10/31/2025"; // arbitray date
RTC rtc(RTC_PIN, DOW, TIME, DATE);
volatile byte tick = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("Starting");
    rtc.begin(&tick);
    Serial.println(rtc.check_alarm(1));
    rtc.set_alarm1(DOW, "03:32:00");
    Serial.println("finished setup");
    Serial.println("Current time");
    Serial.println(rtc.get_time_str());
    Serial.println("Alarm 1 time:");
    Serial.println(rtc.get_alarm1_time(1));
    Serial.println(rtc.check_alarm(1));
}

void loop() {
      Serial.println(rtc.get_time_str());
      Serial.println(rtc.check_alarm(1));
        // if alarm went of, do alarm stuff
    if (tick) {
        // optional serial output
        Serial.println("Alarm is on!");
        return;
    };
}
