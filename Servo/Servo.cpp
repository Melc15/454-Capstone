#include "Servo.h"

void Servo::pulse(int myAngle){
    pulseWidth = (myAngle * 10) + clb_fct; //calibrated equation, 1 for each servo (unless similar)
    digitalWrite(pin, HIGH);  //set servo high
    delayMicroseconds(pulseWidth);  //microsecond pause
    digitalWrite(pin, LOW);  //set servo low
    curr_angle = my_Angle;
}

void Servo::full_ccw(){
    for (int myAngle = curr_angle; myAngle <= MAX_ANGLE; myAngle++){
        pulse(myAngle);
        delay(20);
    }
}

void Servo::full_cw(){
    for (int myAngle = curr_angle; myAngle >= MIN_ANGLE; myAngle--){
        pulse(myAngle);
        delay(20);
    }
}

void Servo::change_pin(int new_pin){
    pin = new_pin;
};
void Servo::change_clb(int new_clb){
    clb_fct = new_clb;
};
int Servo::get_angle(){
    return curr_angle;
};
int Servo::get_clb(){
    return clb_fct;
};