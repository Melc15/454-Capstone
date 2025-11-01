#include "AlarmSystem.h"

const int ELEMENT_COUNT_MAX = 50;

bool AlarmSystem::AddAlarm(String A_t, String A_DOW, bool motor_init[]){
    if(n_motors == -1){
        n_motors = sizeof(motor_init)/sizeof(motor_init[0]) + 1;
    };
    BoolRow M{};
    M.n = sizeof(motor_init)/sizeof(motor_init[0]) + 1;
    for(int i = 0; i < M.n; i++){
        M.b[i] = motor_init[i];
    }
    AlarmRow A{A_DOW, A_t};
    alarms.push_back(A);
    motors.push_back(M);
    if(alarms.size() == 1){
        clock.set_alarm1(A_DOW, A_t);
    }
    return true;
};
bool AlarmSystem::NextAlarm(){
    if(alarms.size() == 0){
        Serial.println("No alarms in queue");
        return false;
    }
    alarms.remove(0);
    motors.remove(0);
    String A_DOW = alarms[0].DOW;
    String A_t = alarms[0].time;
    clock.set_alarm1(A_DOW, A_t);
    return true;
};
bool AlarmSystem::RemoveAlarm(int number){
    if(alarms.size() == 0){
        Serial.println("No alarms in queue");
        return false;
    }
    if(number == 0){
        NextAlarm();
        return true;
    }
    if(number >= alarms.size()){
        Serial.println("Invalid alarm");
        return false;
    }
    alarms.remove(number);
    motors.remove(number);
    return true;
};

void AlarmSystem::PrintAlarms(){
    for (size_t i = 0; i < alarms.size(); i++) {
        Serial.print(alarms[i].DOW);
        Serial.print(" ");
        Serial.println(alarms[i].time);
    }
};

bool* AlarmSystem::GetMotors(){
    bool* M = new bool[motors[0].n];
    for(int i = 0; i < motors[0].n; i++){
        M[i] = motors[0].b[i];
        Serial.println(motors[0].b[i]);
    }
    return M;
}