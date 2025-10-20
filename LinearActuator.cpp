
#include "LinearActuator.h"
LinearActuator::LinearActuator(int& pin_pos, int& pin_neg) : pin_pos(pin_pos), pin_neg(pin_neg){
    pin_mode(pin_pos, OUTPUT);
    pin_mode(pin_neg, OUTPUT);
};
void LinearActuator::push(){
    digitalWrite(pin_pos, HIGH);
    digitalWrite(pin_neg, LOW);
};
void LinearActuator::pull(){
    digitalWrite(pin_pos, LOW);
    digitalWrite(pin_neg, HIGH);
};
void LinearActuator::stop(){
    digitalWrite(pin_pos, HIGH);
    digitalWrite(pin_neg, HIGH);
};