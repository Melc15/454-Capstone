//Very much inspired by https://www.dfrobot.com/blog-1462.html by DFRobot Feb 26 2020
//Additions made by Just Baselmans https://www.youtube.com/justbaselmansYT Jan 23 2023

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Initialize software serial on pins 10 and 11
SoftwareSerial mySoftwareSerial(10, 11);  // RX, TX
DFRobotDFPlayerMini myDFPlayer;
String line;
char command;
int pause = 0;
int repeat = 0;

void setup() {
  // Serial communication with the module
  mySoftwareSerial.begin(9600);
  // Initialize Arduino serial
  Serial.begin(115200);
    if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("DFPlayer Mini initialization failed!");
    while (true);  // Stop here
  }
  myDFPlayer.setTimeOut(500);  // Serial timeout 500ms
  myDFPlayer.volume(25);        // Volume 5
  myDFPlayer.EQ(0);            // Normal equalization
  myDFPlayer.play(1);
}

void loop() {
}




