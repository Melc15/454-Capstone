
#include "LoadCell.h"

LoadCell::LoadCell(int &DOUT_pin, int &SCK_pin): DOUT_pin(DOUT_pin), SCK_pin(SCK_pin), power(true){
    scale.begin(DOUT_pin, SCK_pin);
    scale.power_up();
    scale.set_scale();
    scale.tare();
};
float LoadCell::read_scale(int avg){
    return scale.get_units(avg)
};
float LoadCell::read_scale_raw(){
    return scale.read();
};
void LoadCell::change_power(){
    if(power) {
        scale.power_down();
    }
    else{
        scale.power_up();
    }
    power = !power;
};
void LoadCell::tare(){
    scale.tare();
};
void LoadCell::set_calibration(float &clb_fct_new){
    scale.set_scale(clb_fct_new);
    clb_fct = clb_fct_new;
};

void LoadCell::set_calibration_weight(float &weight){
    float clb_fct_new = calc_calibration(weight);
    scale.set_scale(clb_fct_new);
    clb_fct = clb_fct_new;
};

float LoadCell::calc_calibration(float &weight){
    float reading = scale.get_units(10);
    return reading/weight;
};

float LoadCell::get_clb_fct(){
    return clb_fct;
};