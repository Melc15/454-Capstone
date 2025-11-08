#include "LoadCell.h"

LoadCell::LoadCell(int dout_pin, int sck_pin): DOUT_pin(dout_pin), SCK_pin(sck_pin), power(true), clb_fct(1.0f){
}

void LoadCell::begin(){
    scale.begin(DOUT_pin, SCK_pin);
    scale.power_up();
    scale.set_scale();  // start with 1
    scale.tare();
}

float LoadCell::read_scale(int avg){
    return scale.get_units(avg);
}

long LoadCell::read_scale_raw(){
    return scale.read();
}

void LoadCell::change_power(){
    if (power) {
        scale.power_down();
    } else {
        scale.power_up();
    }
    power = !power;
}

void LoadCell::tare(){
    scale.tare();
}

void LoadCell::set_calibration(float clb_fct_new){
    scale.set_scale(clb_fct_new);
    clb_fct = clb_fct_new;
}

void LoadCell::set_calibration_weight(float weight){
    float clb_fct_new = calc_calibration(weight);
    scale.set_scale(clb_fct_new);
    clb_fct = clb_fct_new;
}

float LoadCell::calc_calibration(float weight){
    float reading = scale.get_units(10);
    return reading / weight;
}

float LoadCell::get_clb_fct() const {
    return clb_fct;
}
