#pragma once
#include <Arduino.h>
#include <Chrono.h>

class IRSensor {
private:
    int sensor_pin;
    int count;
    bool sensor_state;
public:
    IRSensor(int& sensor_pin);
    int get_count() const;
    void incr_count();
    bool check_state();
    int count_breaks(int seconds,  int l_delay = 1000);
    void reset_counter();
    void set_state(bool state);
    void change_pin(int& new_sensor_pin);
};

