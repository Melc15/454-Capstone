#pragma once
#include <string>
#include <format>
#include <DS3231.h>
#include "Arduino.h"


class RTC{
  private:
    DS3231 clock;
    int pin_number;

  public:
    RTC(int pin_number, std::byte& DOW, std::string& time, std::string& date);
    std::string get_time_str();
    std::string get_date_str();
    std::string get_DOW_str();
    std::byte get_DOW_byte();
    bool check_DOW_time(std::byte& DOW, std::string& time);
    bool set_clock(std::string date, std::string DOW, std::string time);
    bool change_pin(int pin_number);

  //// NEED ALARM FUNCTIONS
};
