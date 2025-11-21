#include <RTC.h>
#include <Dispenser.h>

int IR_PIN = 3;
IRSensor irs(IR_PIN);
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

int RX_pin = 10;
int TX_pin = 11;
SoftwareSerial mySoftwareSerial(RX_pin, TX_pin);

Dispenser sys(4, rtc, irs, scale, mySoftwareSerial);
volatile byte tick = 0;

void setup() {
    Serial.begin(115200);
    sys.begin(&tick);
    delay(10);
    Serial.println("Begin");
}

void loop() {
    if (Serial.available() > 0) {
        // 1) read header
        String header = Serial.readStringUntil('\n');
        header.trim();

        // if it's not BEGIN, ignore this line and flush junk
        if (header != "BEGIN") {
            while (Serial.available() > 0) Serial.read();
        } else {
            // 2) read DOW + time
            String DOW_read  = Serial.readStringUntil('\n');
            DOW_read.trim();

            String time_read = Serial.readStringUntil('\n');
            time_read.trim();

            // 3) read 12 ints (from the next line)
            int pills[12];
            for (int i = 0; i < 12; i++) {
                pills[i] = Serial.parseInt();
            }

            // consume remainder of pills line (newline)
            Serial.readStringUntil('\n');

            // 4) optional END check
            String endLine = Serial.readStringUntil('\n');
            endLine.trim();

            if (endLine == "END") {
                sys.AddAlarm(time_read, DOW_read, pills, 1);
                Serial.println("Alarm added");
            } else {
                Serial.println("Bad packet (missing END), ignored");
                // flush whatever is left so we resync cleanly
                while (Serial.available() > 0) Serial.read();
            }
        }
    }
    if (tick) {
        tick = 0;
        Serial.println("Alarm is on!");
        sys.Dispense();
        while(!tick){
            delay(10);
        }
        sys.toggle_song(1);
        tick = 0;
        while(!tick){
            delay(10);
        }
        sys.pills_taken();
        sys.toggle_song(0);
        sys.NextAlarm();
        Serial.println("Remaining alarms:");
        sys.PrintAlarms();
        delay(10);
    };

}

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
