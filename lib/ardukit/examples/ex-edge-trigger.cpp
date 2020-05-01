#include "ardukit.h"
#include <SoftwareSerial.h>

SoftwareSerial mySoftwareSerial(10, 11);
GEdgeTrigger sensor1;

void onRising(GEvent &e)
{
    dmsg("------------------------> Rise triggered");
}

void onFalling(GEvent &e)
{
    dmsg("++++++++++++++++++++++++> Fall triggered");
}

void setup()
{
    Serial.begin(9600);

    sensor1.setThreshoulds(100, 500);
    sensor1.on("rising", onRising);
    sensor1.on("falling", onFalling);
}

void loop()
{
    sensor1.triggerOnEdge(analogRead(A0));
}
