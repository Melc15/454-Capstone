#include <RTC.h>

int RTC_PIN = 5;
byte DOW = 1; // Monday
String TIME = "12:00:00"; // arbitray time
String DATE = "10/20/2025"; // arbitray date

RTC rtc(RTC_PIN, DOW, TIME, DATE);

void setup() {
    Serial.begin(9600);
    rtc.set_alarm1(DOW, "01:45:00");
}

void loop() {
    Serial.println("Is alarm on?");
    Serial.println(rtc.check_alarm(1));
    
}
