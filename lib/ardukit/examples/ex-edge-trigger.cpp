#include "ardukit.h"
#include <SoftwareSerial.h>

using event = adk::event_emitter::event;

SoftwareSerial mySoftwareSerial(10, 11);
GEdgeTrigger sensor1;

void on_rising(event &)
{
    dmsg("------------------------> Rise triggered");
}

void on_falling(event&)
{
    dmsg("++++++++++++++++++++++++> Fall triggered");
}

void setup()
{
    Serial.begin(9600);

    sensor1.set_threshoulds(100, 500);
    sensor1.on("rising", on_rising);
    sensor1.on("falling", on_falling);
}

void loop()
{
    sensor1.detect_edge(analogRead(A0));
}
