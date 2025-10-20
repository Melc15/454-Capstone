///// UNDER CONSTRUCTION
/// VERY BAD IMPLEMENTATION
#include "RTC.h"

RTC::RTC(int pin_number, std::byte& DOW, std::string& time, std::string& date) : pin_number(pin_number){
    // Parsing inputted strings
    std::byte mm, dd, yyyy;
    std::scanf(date.c_str(), "%d/%d/%d", &mm, &dd, &yyyy);
    std::byte hr, min, sec;
    std::scanf(time.c_str(), "%d:%d:%d", &hr, &min, &sec);
    // Initializing things
    clock = DS3231();
    Wire.begin();
    clock.begin();
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

std::string RTC::get_time_str() {
    std::byte hr = clock.getHour();
    std::byte min = clock.getMinute();
    std::byte sec = clock.getSecond();
    return std::format("{:02}:{:02}:{:02}", hr, min, sec);
};

std::string RTC::get_date_str() {
    std::byte mm, dd, yyyy;
    mm = clock.getYear();
    dd = clock.getMonth();
    yyyy = clock.getDate();
    return std::format("{:2d}/{:2d}/{:4d}", mm, dd, yyyy);
};

std::byte RTC::get_DOW_byte() {
    return clock.getDoW();
};

std::string RTC::get_DOW_str() {
    const std::vector<std::string> days = {
        "Invalid Day", // Placeholder for index 0 if using 1-based indexing
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday",
        "Sunday"
    };
    return days[clock.getDoW()];
};

bool RTC::check_DOW_time(std::byte& DOW, std::string& time) {
    std::byte hr_now = clock.getHour();
    std::byte min_now = clock.getMinute();
    std::byte sec_now = clock.getSecond();
    std::byte DOW_now = get_DOW_byte();

    std::byte hr, min, sec;
    std::scanf(time.c_str(), "%d:%d:%d", &hr_now, &min_now, &sec_now);

    if (hr_now < hr && min_now < min && sec_now < sec && DOW == DOW_now) {
        return true;
    }
    return false;
};
bool RTC::set_clock(std::string date, std::string DOW, std::string time) {

    std::byte mm, dd, yyyy;
    std::scanf(date.c_str(), "%d/%d/%d", &mm, &dd, &yyyy);
    std::byte hr, min, sec;
    std::scanf(time.c_str(), "%d:%d:%d", &hr, &min, &sec);
    auto DOW_byte = static_cast<std::byte>(std::stoi(DOW));

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

bool RTC::change_pin(int pin_number): pin_number(pin_number){
};

//// NEED TO IMPLEMENT ALARM FUNCTIONS


