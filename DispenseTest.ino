#include <RTC.h>
#include <Dispenser.h>
#include <ServoMotor.h>
ServoMotor test_motor1(1,100.0);
ServoMotor test_motor2(2, 100.0);
ServoMotor test_motor3(3, 100.0);
ServoMotor test_motors[3] = {test_motor1, test_motor2, test_motor3};

int IR_LED = 1;
int IR_PIN = 2;
IRSensor irs(IR_PIN, IR_LED);

int RTC_PIN = 2;
String DOW = "Friday"; // Friday
String TIME = "03:31:58"; // arbitray time
String DATE = "10/31/2025"; // arbitray date
RTC rtc(RTC_PIN, DOW, TIME, DATE);

Dispenser sys(rtc, test_motors, irs);
volatile byte tick = 0;
int pills_1[3] = {1, 2, 3};
int pills_2[3] = {2, 1, 0};
void setup() {
    Serial.begin(9600);
    rtc.begin(&tick);
    sys.AddAlarm("03:32:06", DOW, pills_1);
    sys.AddAlarm("03:32:16", DOW, pills_2);
    sys.AddAlarm("03:32:08", DOW, pills_1);
    sys.AddAlarm("12:12:12", DOW, pills_2);
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
        ServoMotor* motors = sys.GetMotors();
        sys.NextAlarm();
        Serial.println("Remaining alarms:");
        sys.PrintAlarms();
        delay(10);
    };
}