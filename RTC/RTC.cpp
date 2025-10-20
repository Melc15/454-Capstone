///// UNDER CONSTRUCTION
/// VERY BAD IMPLEMENTATION
#include "RTC.h"

RTC::RTC(int pin_number, byte DOW, String& time, String& date) : pin_number(pin_number), h12Flag(true), pmFlag(true){
    // Parsing inputted strings
    byte mm, dd, yyyy;
    scanf(date.c_str(), "%d/%d/%d", &mm, &dd, &yyyy);
    byte hr, min, sec;
    scanf(time.c_str(), "%d:%d:%d", &hr, &min, &sec);
    // Initializing things
    Wire.begin();
    clock.setClockMode(true);

    // Setting the clock values
    clock.setYear(yyyy);
    clock.setMonth(mm);
    clock.setDate(dd);
    clock.setDoW(DOW);
    clock.setHour(hr);
    clock.setMinute(min);
    clock.setSecond(sec);

};

String RTC::get_time_str() {
    byte hr  = clock.getHour(h12Flag, pmFlag);
    byte min = clock.getMinute();
    byte sec = clock.getSecond();

    String out;
    out.reserve(8);              // "HH:MM:SS"

    out += char('0' + (hr  / 10));
    out += char('0' + (hr  % 10));
    out += ':';
    out += char('0' + (min / 10));
    out += char('0' + (min % 10));
    out += ':';
    out += char('0' + (sec / 10));
    out += char('0' + (sec % 10));

    return out;
}

String RTC::get_date_str() {
    bool century = false;
    unsigned mm   = clock.getMonth(century);
    unsigned dd   = clock.getDate();
    unsigned yyyy = 2000u + clock.getYear();

    String s;
    s.reserve(10);
    if (mm < 10) s += '0'; s += mm; s += '/';
    if (dd < 10) s += '0'; s += dd; s += '/';
    s += (yyyy / 1000) % 10;
    s += (yyyy / 100)  % 10;
    s += (yyyy / 10)   % 10;
    s += (yyyy % 10);

    s.setCharAt(s.length()-4, char('0' + (yyyy / 1000) % 10));
    return s;
}

byte RTC::get_DOW_byte() {
    return clock.getDoW();
};

String RTC::get_DOW_str() {
    // Backing storage for the Vector (fixed-size).
    // We store pointers to flash strings to avoid RAM bloat.
    static const __FlashStringHelper* backing[8];

    // Many Arduino Vector libs use (buffer, capacity) ctor:
    static Vector<const __FlashStringHelper*> days(backing, 8);

    // One-time fill
    if (days.size() == 0) {
        days.push_back(F("Invalid Day")); // index 0 (placeholder if your RTC is 1-based)
        days.push_back(F("Monday"));
        days.push_back(F("Tuesday"));
        days.push_back(F("Wednesday"));
        days.push_back(F("Thursday"));
        days.push_back(F("Friday"));
        days.push_back(F("Saturday"));
        days.push_back(F("Sunday"));
    }

    uint8_t i = get_DOW_byte();
    if (i >= days.size()) i = 0;  // clamp/guard

    // Construct a RAM String from the flash-stored string pointer.
    return String(days[i]);
};

bool RTC::check_DOW_time(byte& DOW, String& time) {
    byte hr_now = clock.getHour(h12Flag, pmFlag);
    byte min_now = clock.getMinute();
    byte sec_now = clock.getSecond();
    byte DOW_now = get_DOW_byte();

    byte hr, min, sec;
    scanf(time.c_str(), "%d:%d:%d", &hr_now, &min_now, &sec_now);

    if (hr_now < hr && min_now < min && sec_now < sec && DOW == DOW_now) {
        return true;
    }
    return false;
};
bool RTC::set_clock(String date, String DOW, String time) {

    byte mm, dd, yyyy;
    scanf(date.c_str(), "%d/%d/%d", &mm, &dd, &yyyy);
    byte hr, min, sec;
    scanf(time.c_str(), "%d:%d:%d", &hr, &min, &sec);
    byte DOW_byte = static_cast<byte>(DOW.toInt());

    // Initializing things
    clock.setClockMode(true);
    clock.setYear(yyyy);
    clock.setMonth(mm);
    clock.setDate(dd);
    clock.setDoW(DOW_byte);
    clock.setHour(hr);
    clock.setMinute(min);
    clock.setSecond(sec);
};

bool RTC::change_pin(int pin_number_new){
    pin_number = pin_number_new;
};

//// NEED TO IMPLEMENT ALARM FUNCTIONS


