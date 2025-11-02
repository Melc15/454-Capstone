#include "ServoMotor.h"
ServoMotor::ServoMotor(): clb_fct(0), pin(-1), curr_angle(0){};
ServoMotor::ServoMotor(int pin, double clb_fct): clb_fct(clb_fct), pin(pin), curr_angle(0){};

void ServoMotor::pulse(int myAngle){
    int pulseWidth = (myAngle * 10) + clb_fct; //calibrated equation, 1 for each servo (unless similar)
    digitalWrite(pin, HIGH);  //set servo high
    delayMicroseconds(pulseWidth);  //microsecond pause
    digitalWrite(pin, LOW);  //set servo low
    curr_angle = myAngle;
}

void ServoMotor::full_ccw(){
    for (int myAngle = curr_angle; myAngle <= MAX_ANGLE; myAngle++){
        pulse(myAngle);
        delay(20);
    }
}

void ServoMotor::full_cw(){
    for (int myAngle = curr_angle; myAngle >= MIN_ANGLE; myAngle--){
        pulse(myAngle);
        delay(20);
    }
}

void ServoMotor::change_pin(int new_pin){
    pin = new_pin;
};
void ServoMotor::change_clb(int new_clb){
    clb_fct = new_clb;
};
int ServoMotor::get_angle(){
    return curr_angle;
};
int ServoMotor::get_clb(){
    return clb_fct;
};