#include "IRSensor.h"

IRSensor::IRSensor(int &sensor_pin): sensor_pin(sensor_pin){
    sensor_state = digitalRead(sensor_pin);
    change_pin(sensor_pin);
    count = 0;
};

int IRSensor::get_count() const {
    return count;
};
void IRSensor::incr_count() {
    count++;
};
bool IRSensor::check_state() {
    sensor_state = digitalRead(sensor_pin);
    return sensor_state;
};

int IRSensor::count_breaks(int seconds,  int l_delay) {
    Chrono timer(Chrono::SECONDS);
    bool prev_state = check_state();
    bool check_bool = not(prev_state);
    while(true) {
        check_state();
        if ((sensor_state == HIGH && prev_state == LOW) && check_bool) {
            incr_count();
        }
        if (sensor_state == LOW && !check_bool)
            check_bool = true;
        if (check_bool)
            prev_state = sensor_state;
        if (timer.elapsed() >= seconds)
            break;
    }
    int final = count;
    return final;
};

void IRSensor::reset_counter() {
    count = 0;
};

void IRSensor::set_state(bool state) {
    digitalWrite(sensor_pin, state);
    sensor_state = state;
};

void IRSensor::change_pin(int& new_sensor_pin){
    sensor_pin = new_sensor_pin;
    pinMode(new_sensor_pin, INPUT_PULLUP);
    sensor_state = true;
};
