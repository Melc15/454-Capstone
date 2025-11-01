#pragma once
#include "Vector.h"
#include "RTC.h"

static const int MAX_ALARMS = 10;
static const int MAX_MOTORS = 6;  // set your max

struct AlarmRow {
    String DOW;
    String time;
};
struct BoolRow{
    byte n = 0;
    bool b[MAX_MOTORS];
};

class AlarmSystem {
private:

    AlarmRow alarms_store[MAX_ALARMS];
    Vector<AlarmRow> alarms;

    BoolRow motors_store[MAX_ALARMS];
    Vector<BoolRow> motors;

    int n_motors;
    RTC clock;
public:
    AlarmSystem() = delete;
    explicit AlarmSystem(RTC rtc): alarms(alarms_store), motors(motors_store), clock(rtc), n_motors(-1){}
    bool AddAlarm(String A_t,  String A_DOW, bool motor_init[]);
    bool NextAlarm();
    void PrintAlarms();
    bool RemoveAlarm(int number);
    bool* GetMotors();
};
