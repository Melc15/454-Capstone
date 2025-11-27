#include <RTC.h>
#include <Dispenser.h>

int IR_PIN = 4;
IRSensor irs(IR_PIN);

int RTC_PIN = 2;
String DOW = "Sunday"; // Friday
String TIME = "23:55:40"; // arbitray time
String DATE = "10/31/2025"; // arbitray date
RTC rtc(RTC_PIN, DOW, TIME, DATE);

int DOUT_pin = 4;
int SCK_pin = 5;
LoadCell scale(DOUT_pin, SCK_pin);

int RX_pin = 10;
int TX_pin = 11;
SoftwareSerial mySoftwareSerial(RX_pin, TX_pin);
DFRobotDFPlayerMini MP3Player;

Dispenser sys(4, rtc, irs, scale, mySoftwareSerial, MP3Player);
volatile byte tick = 0;

void setup() {
    Serial.begin(115200);
    sys.begin(&tick);
    scale.set_calibration(17131.14);
    delay(10);
    sys.toggle_song(0);
}

void loop() {
    if (Serial.available() > 0) {
        // read header
        String header = Serial.readStringUntil('\n');
        header.trim();

        if(header == "CLEAR"){
            sys.ClearAlarms();
        }
        else if (header != "BEGIN") {
            while (Serial.available() > 0) Serial.read();
        } else {
            // read DOW and time
            String DOW_read  = Serial.readStringUntil('\n');

            String time_read = Serial.readStringUntil('\n');

            // read 12 ints
            int pills[12];
            for (int i = 0; i < 12; i++) {
                pills[i] = Serial.parseInt();
                Serial.readStringUntil('\n');
            }
            Serial.readStringUntil('\n');

            // check for end line
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
        sys.Dispense(0);
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
    
    sys.PrintAlarms();

}
