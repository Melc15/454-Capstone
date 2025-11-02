#pragma once
#include "Vector.h"
#include "RTC.h"
#include "ServoMotor.h"
#include "IRSensor.h"
static const int MAX_ALARMS = 10;
static const int MAX_MOTORS = 6;  // set your max

struct AlarmRow {
    String DOW;
    String time;
};
struct CountRow{
    byte n = 0;
    int count[MAX_MOTORS];
};

class Dispenser {
private:
    int n_motors;
    RTC clock;
    IRSensor ir;

    AlarmRow alarms_store[MAX_ALARMS];
    Vector<AlarmRow> alarms;
    CountRow pills_store[MAX_ALARMS];
    Vector<CountRow> pill_counts;

    ServoMotor motors_store[MAX_MOTORS];
    Vector<ServoMotor> motors;

public:
    Dispenser() = delete;
    Dispenser(RTC rtc, ServoMotor motors_i[], IRSensor ir);
    bool AddAlarm(String A_t,  String A_DOW, int pills[]);
    bool NextAlarm();
    void PrintAlarms();
    bool RemoveAlarm(int number);
    ServoMotor* GetMotors();
    int Dispense();
};
