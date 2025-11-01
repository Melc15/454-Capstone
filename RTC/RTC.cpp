///// UNDER CONSTRUCTION
/// VERY BAD IMPLEMENTATION
#include "RTC.h"

volatile byte* RTC::s_tick_ptr = nullptr;

static bool parseDate(const String& s, uint8_t& m, uint8_t& d, uint16_t& y) {
    int im=0, id=0, iy=0;
    if (sscanf(s.c_str(), "%d/%d/%d", &im, &id, &iy) != 3) return false;
    if (im<1||im>12 || id<1||id>31 || iy<0) return false;
    m=(uint8_t)im; d=(uint8_t)id; y=(uint16_t)iy;
    return true;
}

static bool parseTime(const String& s, uint8_t& h, uint8_t& m, uint8_t& se) {
    int ih=0, im=0, is=0;
    if (sscanf(s.c_str(), "%d:%d:%d", &ih, &im, &is) != 3) return false;
    if (ih<0||ih>23 || im<0||im>59 || is<0||is>59) return false;
    h=(uint8_t)ih; m=(uint8_t)im; se=(uint8_t)is;
    return true;
}

String format_str(uint8_t d1, uint8_t d2, uint16_t d3, char delim) {
    // e.g., time: HH:MM:SS -> 8 chars + null
    char buf[11]; // enough for "MM/DD/YYYY" too
    snprintf(buf, sizeof(buf), "%02u%c%02u%c%02u", d1, delim, d2, delim, d3);
    return String(buf);
}

byte DOW_from_str(const String& s){
    String t = s;
    t.trim(); t.toLowerCase();
    if (t.startsWith("mon")) return 1;
    if (t.startsWith("tue")) return 2;
    if (t.startsWith("wed")) return 3;
    if (t.startsWith("thu")) return 4;  // covers thu/thur/thurs
    if (t.startsWith("fri")) return 5;
    if (t.startsWith("sat")) return 6;
    if (t.startsWith("sun")) return 7;
    return 0; // invalid
}

RTC::RTC(int pin_number, byte mm, byte dd, byte yyyy, byte hr, byte min, byte sec, byte DOW)
: mm_init(mm), dd_init(dd), yyyy_init(yyyy), min_init(min), hr_init(hr), sec_init(sec), DOW_init(DOW), interp_pin(pin_number), h12Flag(0), pmFlag(0), A1_on(0), A2_on(0){
}

RTC::RTC(int pin_number, byte DOW, String& time, String& date): DOW_init(DOW), interp_pin(pin_number), h12Flag(0), pmFlag(0), A1_on(0), A2_on(0)
{
    uint8_t mm_i, dd_i, hr_i, min_i, sec_i;
    uint16_t yyyy_i;
    parseDate(date, mm_i, dd_i, yyyy_i);
    parseTime(time, hr_i,  min_i, sec_i);
    mm_init = mm_i;
    dd_init = dd_i;
    yyyy_init = yyyy_i;
    hr_init = hr_i;
    min_init = min_i;
    sec_init = sec_i;
}
RTC::RTC(int pin_number, String& DOW, String& time, String& date): interp_pin(pin_number), h12Flag(0), pmFlag(0), A1_on(0), A2_on(0)
{
    byte DOW_byte = DOW_from_str(DOW);
    byte mm, dd, hr, min, sec;
    uint16_t yyyy;
    parseDate(date, mm, dd, yyyy);
    parseTime(time, hr, min, sec);
    DOW_init = DOW_byte;
    mm_init = mm;
    dd_init = dd;
    yyyy_init = yyyy;
    hr_init = hr;
    min_init = min;
    sec_init = sec;
}

bool RTC::begin(volatile byte* tick_var) {
    s_tick_ptr = tick_var;
    Wire.begin();
    clock.setClockMode(false);
    clock.setYear(yyyy_init % 100);  // DS3231 2-digit year
    clock.setMonth(mm_init);
    clock.setDate(dd_init);
    clock.setDoW(DOW_init);
    clock.setHour(hr_init);
    clock.setMinute(min_init);
    clock.setSecond(sec_init);

    // Make sure both alarms are inert and flags are clear
    clock.turnOffAlarm(1);
    clock.turnOffAlarm(2);
    clock.checkIfAlarm(1);  // clears A1 flag
    clock.checkIfAlarm(2);  // clears A2 flag

    // Proactively prevent A2 from ever matching (minute = 0xFF) and disable it
    clock.setA2Time(0, 0, 0xFF, 0b01100000, false, false, false);
    clock.turnOffAlarm(2);
    clock.checkIfAlarm(2);
    pinMode(interp_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interp_pin),  RTC::isrThunk, FALLING);
    return true;
}

 void RTC::isrThunk() {
    if (RTC::s_tick_ptr) *RTC::s_tick_ptr = 1;  // update the sketch variable
}

String RTC::get_time_str() {
    byte hr  = clock.getHour(h12Flag, pmFlag);
    byte min = clock.getMinute();
    byte sec = clock.getSecond();
    return format_str(hr, min, sec, ':');
}

String RTC::get_date_str() {
    bool century = false;
    byte mm = clock.getMonth(century);
    byte dd = clock.getDate();
    byte yy = clock.getYear();
    return format_str(mm, dd, yy, '/');
}

byte RTC::get_DOW_byte() {
    return clock.getDoW();
};

String RTC::get_DOW_str() {
    static const __FlashStringHelper* const names[] = {
            F("Invalid Day"),
            F("Monday"), F("Tuesday"), F("Wednesday"),
            F("Thursday"), F("Friday"), F("Saturday"), F("Sunday")
    };
    uint8_t i = get_DOW_byte();
    if (i > 7) i = 0;
    return String(names[i]);
}

bool RTC::set_clock(String date, byte DOW, String time) {

    byte mm, dd, yyyy;
    scanf(date.c_str(), "%d/%d/%d", &mm, &dd, &yyyy);
    byte hr, min, sec;
    scanf(time.c_str(), "%d:%d:%d", &hr, &min, &sec);

    clock.setYear(yyyy);
    clock.setMonth(mm);
    clock.setDate(dd);
    clock.setDoW(DOW);
    clock.setHour(hr);
    clock.setMinute(min);
    clock.setSecond(sec);
    return true;
};

bool RTC::change_pin(int pin_number_new){
    detachInterrupt(digitalPinToInterrupt(interp_pin));
    interp_pin = pin_number_new;
    pinMode(interp_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interp_pin), RTC::isrThunk, FALLING);
    return true;
};
void RTC::set_alarm1(String& DOW, String time, bool A1dDy){
    byte DOW_byte  = DOW_from_str(DOW);
    set_alarm1(DOW_byte, time, A1dDy);
    return;

};
void RTC::set_alarm2(String& DOW, String time, bool A1dDy){
    byte DOW_byte = DOW_from_str(DOW);
    set_alarm2(DOW_byte, time, A1dDy);
};

void RTC::set_alarm1(byte DOW, String time, bool A1dDy){
    clear_alarm(1);
    byte hr, min, sec;
    if (!parseTime(time, hr,  min, sec)) { Serial.println("Bad time"); }
    byte AlarmBits = 0b00000000; // alarm every week and it checks the hr_init, min_init ,sec_init
    clock.setA1Time(DOW, hr, min, sec, AlarmBits, A1dDy, h12Flag, pmFlag);
    toggle_alarm(1);
};

void RTC::set_alarm2(byte DOW, String time, bool A2dDy){
    clear_alarm(2);
    byte hr, min, sec;
    if (!parseTime(time, hr, min, sec)) { Serial.println("Bad time"); }
    byte AlarmBits = 0b00000000; // alarm every week and it checks the hr_init, min_init ,sec_init
    clock.setA2Time(DOW, hr, min, AlarmBits, A2dDy, h12Flag, pmFlag);
    toggle_alarm(2);
};

String RTC::get_alarm1_time(bool PM){
    String out;
    byte DOW, hr, min, sec, AlarmBits;
    bool A1Dy;
    clock.getA1Time(DOW, hr, min, sec, AlarmBits, A1Dy, h12Flag, pmFlag);
    return format_str(hr, min, sec, ':');
}

String RTC::get_alarm2_time(bool PM){
    String out;
    byte DOW, hr, min, sec, AlarmBits;
    sec = 0;
    bool A1Dy;
    clock.getA2Time(DOW, hr, min, AlarmBits, A1Dy, h12Flag, pmFlag);
    return format_str(hr, min, sec, ':');
};

bool RTC::check_alarm(int alarm_number){
    return clock.checkIfAlarm(alarm_number, false);
};

void RTC::clear_alarm(int alarm_number){
    if(alarm_number != 1 && alarm_number != 2){
        Serial.println("Invalid alarm number");
        return;
    }
    clock.checkIfAlarm(alarm_number);
}

void RTC::toggle_alarm(int alarm_number){
    if(alarm_number == 1){
        toggle_A1();
    }
    else if(alarm_number == 2){
        toggle_A2();
    }
    else{
        Serial.println("Invalid alarm number");
        return;
    }
}

void RTC::toggle_A1(){
    if(A1_on){
        clock.turnOffAlarm(1);
    }
    else{
        clock.turnOnAlarm(1);
    }
    A1_on = A1_on;
};
void RTC::toggle_A2(){
    if(A2_on){
        clock.turnOffAlarm(2);
        }
    else{
        clock.turnOnAlarm(2);
    }
    A2_on = !A2_on;
};

//// NEED TO IMPLEMENT ALARM FUNCTIONS


