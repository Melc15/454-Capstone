#include <RTC.h>
#include <IRSensor.h>
#include <LinearActuator.h>

int IR_LED = 1;
int IR_PIN = 2;
int LIN_POS = 3;
int LIN_NEG = 4;
int RTC_PIN = 5;
int DOW = 1; // Monday
String TIME = "12:00:00"; // arbitray time
String DATE = "10/20/2025"; // arbitray date

  RTC rtc(RTC_PIN, DOW, TIME, DATE);
  IRSensor irs(IR_PIN, IR_LED);
  LinearActuator lin(LIN_POS, LIN_NEG);
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("Current Time:");
  Serial.println(rtc.get_time_str());
  Serial.println("Current Date:");
  Serial.println(rtc.get_date_str());
  Serial.println("Current DoW");
  Serial.println(rtc.get_DOW_str());

  bool stop_count = false;
  Serial.println("Counting...");
  irs.check_state_cont(stop_count);
  delay(1000); // wait a second
  stop_count = true;
  Serial.println(irs.get_count());

  Serial.println("Pushing...");
  delay(1000);
  lin.push();
  Serial.println("Pulling...");
  delay(1000);
  lin.pull();
  lin.stop();
  
  delay(5000);
}
