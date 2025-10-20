//
// Created by Kevin Gao on 2025-10-19.
//

#ifndef IRSENSOR_H
#define IRSENSOR_H
#include <Arduino.h>

class IRSensor {
private:
    int sensor_pin;
    int led_pin;
    int count;
    bool sensor_state;
public:
    IRSensor(int& sensor_pin, int& led_pin);
    int get_count();
    void incr_count();
    bool check_state();
    bool check_state_cont(bool& stop);
    void reset_counter();
    void set_state(bool state);
    void change_pin(int& new_sensor_pin, int& new_led_pin);
};



#endif //IRSENSOR_H
