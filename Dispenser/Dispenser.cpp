#include "Dispenser.h"

Dispenser::Dispenser(int n_motors, RTC rtc, IRSensor ir, LoadCell scale, SoftwareSerial softserial, DFRobotDFPlayerMini mp3serial): MP3Player(mp3serial), mp3Serial(softserial), alarms_1(alarms_store1), alarms_2(alarms_store2), pill_counts(pills_store), driver(), clock(rtc), ir(ir), n_motors(n_motors), pwm(112, 550), scale(scale), pills_weight(-1)
{
   mp3Serial.begin(115200);
}


int DOW_to_int(const String &DOW) {
    if (DOW == "Sunday"  || DOW == "Sun")  return 0;
    if (DOW == "Monday"  || DOW == "Mon")  return 1;
    if (DOW == "Tuesday" || DOW == "Tue")  return 2;
    if (DOW == "Wednesday" || DOW == "Wed") return 3;
    if (DOW == "Thursday"  || DOW == "Thu") return 4;
    if (DOW == "Friday"    || DOW == "Fri") return 5;
    if (DOW == "Saturday"  || DOW == "Sat") return 6;
    return -1;
}

void Dispenser::begin(volatile byte* tick){
    scale.begin();
    clock.begin(tick);
    driver.resetDevices();
    driver.init();
    driver.setPWMFrequency(50);
    MP3Player.begin(mp3Serial);
    MP3Player.setTimeOut(500);
    MP3Player.volume(30);
    MP3Player.EQ(0);
}
bool Dispenser::AddAlarm(String A_t, String A_DOW, int pills[], int wait_before){
    if(DOW_to_int(A_DOW) == -1){
        Serial.println("Invalid DOW Entered");
        return false;
    }
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
    for(int i = 0; i < counts.n - 1; i++){
        delay(1000);
        total_count += counts.count[i];
        for(int j = 0; j < counts.count[i]; j++) {
            for(int k = 0; k >= -180; k--){
                driver.setChannelPWM(i, pwm.pwmForAngle(k));
            }
            ir.count_breaks(2, 0);
            for(int k = -180; k <= 180; k++){
                driver.setChannelPWM(i, pwm.pwmForAngle(k));
                delay(2);
            }
        }
        if(counts.count[i] != 0){
            if(ir.get_count() - total_count != counts.count[i]){
                Serial.println("Misdispense Detected, Diverting to Reject.");
                delay(1000);
                driver.setChannelPWM(counts.n, pwm.pwmForAngle(100));
                delay(1000);
                driver.setChannelPWM(counts.n, pwm.pwmForAngle(0));
            }
            else{
                delay(1000);
                driver.setChannelPWM(counts.n, pwm.pwmForAngle(-100));
                delay(1000);
                driver.setChannelPWM(counts.n, pwm.pwmForAngle(0));
            }
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
bool Dispenser::ClearAlarms(){
    if(alarms_1.size() == 0){
        Serial.println("No alarms in queue");
        return false;
    }

    for(int i = 0; i < alarms_1.size(); i++) {
        alarms_1.remove(i);
        alarms_2.remove(i);
        pill_counts.remove(i);
    }
    return true;
}
void Dispenser::toggle_song(int i){
    MP3Player.play(i);
}

bool Dispenser::PillsTaken(){
    if(pills_weight - scale.read_scale(100) <= 0.2){
        return true;
    }
    Serial.println('Pills NOT taken!');
    return false;
}

void Dispenser::DumpAlarmsToSerial()
{
    // use alarms_2 for the “real” dispense time + pill_counts
    for (size_t i = 0; i < alarms_2.size(); ++i) {
        Serial.println("BEGIN");

        Serial.println(alarms_2[i].DOW);    // e.g. "Sunday"
        Serial.println(alarms_2[i].time);   // "HH:MM:SS"

        CountRow &C = pill_counts[i];
        for (int j = 0; j < C.n; ++j) {
            Serial.print(C.count[j]);
            if (j < C.n - 1) Serial.print(' ');
        }
        Serial.println();                   // end pill line

        Serial.println("END");
    }
    Serial.println("DONE");                 // sentinel
}
