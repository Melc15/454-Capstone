#include "dow.h"
DOW::DOW(){
}
DOW::DOW(std::string time){
    Alarm alarm = {time, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    alarms.push_back(alarm);
};

void DOW::add_alarm(std::string time){
    Alarm alarm = {time, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    alarms.push_back(alarm);
};

void DOW::set_pills(std::string time, int cmp, int count){
        for(Alarm& x : alarms){
            if(x.time == time){
                x.pill_counts[cmp] = count;
                break;
        }
    }
};
void DOW::reset_pills(std::string time){
    for(Alarm& x : alarms){
        if(x.time == time){
            x.pill_counts.fill(0);
            break;
        }
    }
};

void DOW::remove_alarm(std::string time){
    int i = 0;
    for(Alarm& x : alarms){
        if(x.time == time){
            alarms.erase(alarms.begin() + i);
            break;
        }
        i++;
    }
};
