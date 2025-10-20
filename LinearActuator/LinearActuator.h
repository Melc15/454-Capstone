#pragma once
#include "Arduino.h"

class LinearActuator {
private:
    int pin_pos;
    int pin_neg;
public:
    LinearActuator(int& pin_pos, int& pin_neg);
    void push();
    void pull();
    void stop();
};


