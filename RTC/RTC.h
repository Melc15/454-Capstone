#pragma once
#include <Vector.h>
#include <DS3231.h>
#include <Arduino.h>


class RTC{
  private:
    DS3231 clock;
    int pin_number;
    bool h12Flag;
    bool pmFlag;
  public:
    RTC(int pin_number, byte DOW, String& time, String& date);
    String get_time_str();
    String get_date_str();
    String get_DOW_str();
    byte get_DOW_byte();
    bool check_DOW_time(byte& DOW, String& time);
    bool set_clock(String date, String DOW, String time);
    bool change_pin(int pin_number_new);

  //// NEED ALARM FUNCTIONS
};
