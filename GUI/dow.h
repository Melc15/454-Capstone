#ifndef DOW_H
#define DOW_H
#include <string>
#include <vector>
#include <array>

struct Alarm{
    std::string time;
    std::array<int, 12> pill_counts;
};

class DOW{
public:
    std::vector<Alarm> alarms;
    DOW();
    DOW(std::string time);
    void add_alarm(std::string time);
    void set_pills(std::string time, int cmp, int count);
    void reset_pills(std::string time);
    void remove_alarm(std::string time);

};



#endif // DOW_H
