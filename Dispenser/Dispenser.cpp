#include "Dispenser.h"

Dispenser::Dispenser(int n_motors, RTC rtc, IRSensor ir, LoadCell scale): alarms_1(alarms_store1), alarms_2(alarms_store2), pill_counts(pills_store), driver(), clock(rtc), ir(ir), n_motors(n_motors), pwm(112, 550), scale(scale), pills_weight(-1){
}

void Dispenser::begin(volatile byte* tick){
    scale.begin();
    clock.begin(tick);
    driver.resetDevices();
    driver.init();
    driver.setPWMFrequency(50);
}

bool Dispenser::AddAlarm(String A_t, String A_DOW, int pills[], int wait_before){
    String A_t1 = clock.subtime_alarm(A_t, wait_before);
    CountRow C{};
    C.n = n_motors;
    for(int i = 0; i < C.n; i++){
        C.count[i] = pills[i];
    }
    AlarmRow A1{A_DOW, A_t1};
    AlarmRow A2{A_DOW, A_t};
    alarms_1.push_back(A1);
    alarms_2.push_back(A2);
    pill_counts.push_back(C);
    if(alarms_1.size() == 1){
        clock.set_alarm1(A_DOW, A_t1);
        clock.set_alarm2(A_DOW, A_t);
    }
    return true;
};
bool Dispenser::NextAlarm(){
    if(alarms_1.size() == 0){
        Serial.println("No alarms_1 in queue");
        return false;
    }
    if(alarms_1.size() == 1){
        Serial.println("No next alarm");
        return false;
    }
    alarms_1.remove(0);
    alarms_2.remove(0);
    pill_counts.remove(0);
    String A_DOW = alarms_1[0].DOW;
    String A_t1 = alarms_1[0].time;
    clock.set_alarm1(A_DOW, A_t1);
    String A_t2 = alarms_2[0].time;
    clock.set_alarm2(A_DOW, A_t2);

    return true;
};
bool Dispenser::RemoveAlarm(int number){
    if(alarms_1.size() == 0){
        Serial.println("No alarms_1 in queue");
        return false;
    }
    if(number == 0){
        NextAlarm();
        return true;
    }
    if(number >= alarms_1.size()){
        Serial.println("Invalid alarm");
        return false;
    }
    alarms_1.remove(number);
    alarms_2.remove(number);
    pill_counts.remove(number);
    return true;
};

void Dispenser::PrintAlarms(){
    for (size_t i = 0; i < alarms_1.size(); i++) {
        Serial.print(alarms_1[i].DOW);
        Serial.print(" ");
        Serial.println(alarms_1[i].time);
        Serial.print(" ");
        Serial.println(alarms_2[i].time);
    }
};


int Dispenser::Dispense(int wait_after){
    CountRow counts = pill_counts[0];
    ir.reset_counter();
    scale.tare();
    int total_count = 0;
    for(int i = 0; i < counts.n-1; i++){
        total_count += counts.count[i];
        for(int j = 0; j < counts.count[i]; j++) {
            driver.setChannelPWM(i, pwm.pwmForAngle(180));
            ir.count_breaks(10, 0);
            driver.setChannelPWM(i, pwm.pwmForAngle(-180));
            delay(2000);
        }
        if(ir.get_count() - total_count != counts.count[i]){
            Serial.println("Misdispense Detected, Diverting to Reject.");
            delay(1000);
            driver.setChannelPWM(n_motors-1, pwm.pwmForAngle(60));
            delay(1000);
            driver.setChannelPWM(n_motors-1, pwm.pwmForAngle(-60));
        }
    }
    if(total_count != ir.get_count()){
        Serial.println("IR Beam Count: ");
        Serial.print(ir.get_count());
        Serial.println("Total Pills: ");
        Serial.print(total_count);
    }
    pills_weight = scale.read_scale(100);
    String A_DOW = alarms_2[0].DOW;
    String A_t = clock.addtime_alarm(alarms_2[0].time, wait_after);
    clock.set_alarm1(A_DOW, A_t);
    return ir.get_count();
}

bool Dispenser::pills_taken(){
    if(pills_weight - scale.read_scale(100) <= 0.2){
        return true;
    }
    Serial.println('Pills NOT taken!');
    return false;
}

