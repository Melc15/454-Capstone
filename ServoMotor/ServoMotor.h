#pragma once
#include "Arduino.h"
class ServoMotor {
private:
    double clb_fct;
    int curr_angle;
    int pin;
public:
    int MAX_ANGLE = 180;
    int MIN_ANGLE = 0;
    ServoMotor();
    ServoMotor(int pin, double clb_fct);
    void pulse(int myAngle);
    void full_ccw();
    void full_cw();
    void change_pin(int new_pin);
    void change_clb(int new_clb);
    int get_angle();
    int get_clb();
};


