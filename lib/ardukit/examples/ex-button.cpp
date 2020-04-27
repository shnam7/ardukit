#include "ardukit.h"

void onButtonEvent(GEvent &e)
{
    dmsg("Button Event: %s", e.eventName());
};

GButton b1(8, INPUT_PULLUP);

void setup()
{
    Serial.begin(128000);
    b1.enable();
    b1.on("press", onButtonEvent);
    b1.on("releas", onButtonEvent);
}

void loop()
{
    adk::run();
}
