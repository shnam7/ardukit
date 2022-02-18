#include "ardukit.h"
#include <SoftwareSerial.h>

using namespace adk;

SoftwareSerial mySoftwareSerial(10, 11);
EdgeTrigger sensor1;

void on_rising(EventEmitter::event&)
{
    dmsg("------------------------> Rise triggered");
}

void on_falling(EventEmitter::event&)
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
