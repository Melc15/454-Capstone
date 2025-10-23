#include "IRSensor.h"

IRSensor::IRSensor(int &sensor_pin, int &led_pin): sensor_pin(sensor_pin), led_pin(led_pin){
    sensor_state = true;
    change_pin(sensor_pin, led_pin);
    count = 0;
};

int IRSensor::get_count() {
    return count;
};
void IRSensor::incr_count() {
    count++;
};
bool IRSensor::check_state() {
    sensor_state = digitalRead(sensor_pin);
    if (sensor_state == LOW) {
        count++;
        digitalWrite(led_pin, HIGH);
    }
    else {
        digitalWrite(led_pin, LOW);
    }
    return sensor_state;
};

bool IRSensor::check_state_cont(bool& stop) {
    while (!stop) {
        sensor_state = digitalRead(sensor_pin);
        if (sensor_state == LOW) {
            count = count + 1;
            digitalWrite(led_pin, HIGH);
        }
        else {
            digitalWrite(led_pin, LOW);
        }
    }
    return sensor_state;

};

void IRSensor::reset_counter() {
    count = 0;
};
void IRSensor::set_state(bool state) {
    digitalWrite(sensor_pin, state);
    sensor_state = state;
};

void IRSensor::change_pin(int& new_sensor_pin, int& new_led_pin){
    sensor_pin = new_sensor_pin;
    led_pin = new_led_pin;
    pinMode(new_led_pin, OUTPUT);
    pinMode(new_sensor_pin, INPUT);
    digitalWrite(new_sensor_pin, HIGH);
};
