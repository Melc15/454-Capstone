#pragma once
#include <Arduino.h>
#include <HX711.h>


class LoadCell {
private:
    int DOUT_pin;
    int SCK_pin;
    bool power;
    HX711 scale;
    float clb_fct;
public:
    LoadCell(int &DOUT_pin, int &SCK_pin);
    float read_scale(int avg = 5);
    float read_scale_raw();
    void change_power();
    void tare();
    void set_calibration(float &clb_fct_new);
    void set_calibration_weight(float &weight);
    float calc_calibration(float &weight);
    float get_clb_fct();
};

