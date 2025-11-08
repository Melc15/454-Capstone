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
    bool A1_on;
    bool A2_on;
    bool h12Flag;
    bool pmFlag;
    byte mm_init;
    byte dd_init;
    byte yyyy_init;
    byte hr_init;
    byte min_init;
    byte sec_init;
    byte DOW_init;
    void toggle_A1();
    void toggle_A2();

  public:
    RTC(int pin_number, byte mm, byte dd, byte yyyy, byte hr, byte min, byte sec, byte DOW);
    RTC(int pin_number, byte DOW, String& time, String& date);
    RTC(int pin_number, String& DOW, String& time, String& date);
    bool begin(volatile byte* tick_var);
    String get_time_str();
    String get_date_str();
    String get_DOW_str();
    byte get_DOW_byte();
    bool set_clock(String date, byte DOW, String time);
    bool change_pin(int pin_number_new);
    void set_alarm1(String& DOW, String time, bool A1dDy = true);
    void set_alarm2(String& DOW, String time, bool A1dDy = true);
    void set_alarm1(byte DOW, String time, bool A1dDy = true);
    void set_alarm2(byte DOW, String time, bool A2dDy = true);
    String get_alarm1_time(bool PM);
    String get_alarm2_time(bool PM);
    bool check_alarm(int alarm_number);
    void clear_alarm(int alarm_number);
    void toggle_alarm(int alarm_number);
    String addtime_alarm(String time, int minutes);
    String subtime_alarm(String time, int minutes);
};
