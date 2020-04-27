#include "ardukit.h"
// #include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

SoftwareSerial mySoftwareSerial(10,11);
// DFRobotDFPlayerMini myDFPlayer;

void onRising(GEvent &e) {
    dmsg("------------------------> Rige triggered");
}
void onFalling(GEvent &e) {
    dmsg("++++++++++++++++++++++++> Fall triggered");
}

GEdgeTrigger sensor1;



void setup() {
    Serial.begin(9600);

    sensor1.setThreshoulds(100, 500);
    sensor1.on("rising", onRising);
    sensor1.on("falling", onFalling);

    // if (!myDFPlayer.begin(mySoftwareSerial)) {
    //     Serial.println("Software Serial Error.");
    //     while (true);
    // }
}

void loop() {
    // dmsg("Val=%d", );
    sensor1.triggerOnEdge( analogRead(A0) );
}
