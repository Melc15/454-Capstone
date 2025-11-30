#include <RTC.h>
#include <Dispenser.h>

int IR_PIN = 1;
IRSensor irs(IR_PIN);

int RTC_PIN = 2;
DateTime temp(__DATE__, __TIME__);
byte dd = temp.day();
byte mm = temp.month();
byte yyyy = temp.year();
byte hr = temp.hour();
byte min = temp.minute();
byte sec = temp.second();

uint32_t epoch = temp.unixtime();        // seconds since 1970-01-01
uint32_t days  = epoch / 86400UL;        // whole days since epoch
byte DOW_0_6 = (days + 4) % 7;           // 0=Sunday, 1=Monday, ... 6=Saturday
byte DOW = DOW_0_6 + 1;

RTC rtc(RTC_PIN, mm, dd, yyyy, hr, min, sec, DOW_0_6);

int DOUT_pin = 5;
int SCK_pin = 6;
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
    Serial.println("Setup Complete");
    delay(10);
}

void loop() {
    if (Serial.available() > 0) {
        String header = Serial.readStringUntil('\n');
        header.trim();
        if(header == "DUMP"){
            sys.DumpAlarmsToSerial();
        }
        else if(header == "CLEAR"){
            sys.ClearAlarms();
        }
        else if (header == "PRINT") {
            Serial.print("Current RTC: ");
            Serial.print(rtc.get_DOW_str());
            Serial.print(' ');
            Serial.println(rtc.get_time_str());
            Serial.println("Alarms:");
            sys.PrintAlarms();          
        }
        else if (header == "BEGIN") {
            String DOW_read  = Serial.readStringUntil('\n');
            String time_read = Serial.readStringUntil('\n');

            DOW_read.trim();       // remove \r / spaces
            time_read.trim();    


            int pills[12];
            for (int i = 0; i < 12; i++) {
                String line = Serial.readStringUntil('\n');
                line.trim();
                pills[i] = line.toInt();
            }
            Serial.readStringUntil('\n');
            
            String endLine = Serial.readStringUntil('\n');
            endLine.trim();


            if (endLine == "END") {
                sys.AddAlarm(time_read, DOW_read, pills, 1);
            } else {
                while (Serial.available() > 0) Serial.read();
            }
        }
    }
    
    if (tick) {
        tick = 0;
        sys.Dispense(1);
        while(!tick){
            delay(10);
        }
        sys.toggle_song(1);
        tick = 0;
        while(!tick){
            delay(10);
        }
        sys.PillsTaken();
        sys.toggle_song(0);
        sys.NextAlarm();
        Serial.println("Remaining alarms:");
        sys.PrintAlarms();
        delay(10);
    };
}
