#include "Dispenser.h"


Dispenser::Dispenser(RTC rtc, Servo motors_i[], IRSensor ir): alarms(alarms_store), pills_counts(pills_store), clock(rtc), ir(ir){
    n_motors = sizeof(motors_i)/sizeof(motors_i[0]) + 1;;
    motors_store = new Servo[n_motors];
    motors.setStorage(motors_store);
    for(int i = 0; i < n_motors; i++){
        motors.push_back(motors_i[i]);
    }

}
bool Dispenser::AddAlarm(String A_t, String A_DOW, int pills[]){
    if(sizeof(pills)/sizeof(pills[0]) + 1 != n_motors){
        Serial.println("Pills != Number of motors")
        return false;
    }
    CountRow C{};
    C.n = n_motors;
    for(int i = 0; i < C.n; i++){
        C.count[i] = pills[i];
    }
    AlarmRow A{A_DOW, A_t};
    alarms.push_back(A);
    pill_counts.push_back(C);
    if(alarms.size() == 1){
        clock.set_alarm1(A_DOW, A_t);
    }
    return true;
};
bool Dispenser::NextAlarm(){
    if(alarms.size() == 0){
        Serial.println("No alarms in queue");
        return false;
    }
    alarms.remove(0);
    pill_counts.remove(0);
    String A_DOW = alarms[0].DOW;
    String A_t = alarms[0].time;
    clock.set_alarm1(A_DOW, A_t);
    return true;
};
bool Dispenser::RemoveAlarm(int number){
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

void Dispenser::PrintAlarms(){
    for (size_t i = 0; i < alarms.size(); i++) {
        Serial.print(alarms[i].DOW);
        Serial.print(" ");
        Serial.println(alarms[i].time);
    }
};

bool* Dispenser::GetMotors(){
    bool* M = new bool[motors[0].n];
    for(int i = 0; i < motors[0].n; i++){
        M[i] = motors[0].b[i];
        Serial.println(motors[0].b[i]);
    }
    return M;
}

int Dispenser::Dispense(){
    CountRow counts = pill_counts[0];
    ir.reset_counter();
    int total_count = 0;
    for(int i = 0; i < count.n; i++){
        total_count += counts.count[i];
        for(int j = 0; j < counts.count[i]; j++) {
            motors[0].full_ccw;
            ir.count_breaks(10, 1);
            motors[0].full_cw;
        }
    }
    if(total_counts != ir.get_count()){
        Serial.println("Misdispense Detected!");
        Serial.println("IR Beam Count: ");
        Serial.print(ir.get_count());
        Serial.println("Total Pills: ");
        Serialprint(total_counts);
    }
    return ir.get_count();
}