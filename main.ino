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

Dispenser sys(12, rtc, irs, scale);
volatile byte tick = 0;

void setup() {
    Serial.begin(115200);
    sys.begin(&tick);
    delay(10);
}

void loop() {
    if (Serial.available() > 0) {
        String DOW = Serial.readStringUntil('\n');  // Read until newline
        String time = Serial.readStringUntil('\n');
        int pills[12];
        for(int i = 0, i < 12, i++){
          pills[i] = Serial.parseInt();
        }
        Serial.readStringUntil('\n');
        sys.AddAlarm(time, DOW, pills, 1);
    }
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
