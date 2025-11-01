#pragma once
#include <Vector.h>
#include <DS3231.h>
#include <Arduino.h>


class RTC{
  private:
    DS3231 clock;
    int interp_pin;
    static volatile byte* s_tick_ptr;
    static void isrThunk();
    bool h12Flag;
    bool pmFlag;
    byte mm_init;
    byte dd_init;
    byte yyyy_init;
    byte hr_init;
    byte min_init;
    byte sec_init;
    byte DOW_init;
  public:
    RTC(int pin_number, byte mm, byte dd, byte yyyy, byte hr, byte min, byte sec, byte DOW);
    RTC(int pin_number, byte DOW, String& time, String& date);
    bool begin(volatile byte* tick_var);
    String get_time_str();
    String get_date_str();
    String get_DOW_str();
    byte get_DOW_byte();
    bool check_DOW_time(byte& DOW, String& time);
    bool set_clock(String date, byte DOW, String time);
    bool change_pin(int pin_number_new);
    void set_alarm1(byte DOW, String time, bool A1dDy = true);
    void set_alarm2(byte DOW, String time, bool A2dDy = true);
    String get_alarm1_time(bool PM);
    String get_alarm2_time(bool PM);
    bool check_alarm(int alarm_number);
};
