#pragma once
#include "Arduino.h"
class Servo {
private:
    double clb_fct;
    int curr_angle;
    int pin;
public:
    int MAX_ANGLE = 180;
    int MIN_ANGLE = 0;
    Servo(pin, clb_fct):clb_fct(clb_fct), pin(pin), curr_angle(MIN_ANGLE){};
    void pulse(int myAngle);
    void full_ccw();
    void full_cw();
    void change_pin(int new_pin);
    void change_clb(int new_clb);
    int get_angle();
    int get_clb();
};


