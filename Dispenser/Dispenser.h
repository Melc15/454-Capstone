#pragma once
#include "Vector.h"
#include "RTC.h"
#include "ServoMotor.h"
#include "IRSensor.h"
#include "LoadCell.h"
#include "PCA9685.h"
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
    LoadCell scale;
    double pills_weight;

    AlarmRow alarms_store1[MAX_ALARMS];
    Vector<AlarmRow> alarms_1;

    AlarmRow alarms_store2[MAX_ALARMS];
    Vector<AlarmRow> alarms_2;

    CountRow pills_store[MAX_ALARMS];
    Vector<CountRow> pill_counts;

    PCA9685 driver;
    PCA9685_ServoEval pwm;

public:
    Dispenser() = delete;
    Dispenser(int n_motors, RTC rtc, IRSensor ir, LoadCell scale);
    void begin(volatile byte* tick);
    bool AddAlarm(String A_t,  String A_DOW, int pills[], int wait_before = 3);
    bool NextAlarm();
    void PrintAlarms();
    bool RemoveAlarm(int number);
    int Dispense(int wait_after = 5);
    bool PillsTaken();
    void DumpAlarmsToSerial();
};
