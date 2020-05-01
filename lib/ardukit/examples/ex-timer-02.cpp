#include "ardukit.h"

int brightness = 0;
int delta = 5;

void setLED(GEvent &e)
{
    dmsg("LED brightness: %d", brightness);
    brightness += delta;
    if (brightness == 255 || brightness == 0) delta = -delta;
    GTimer::setTimeout(setLED, 10);
}

void setup()
{
    Serial.begin(128000);
    GTimer::setTimeout(setLED);
}

void loop()
{
    adk::run();
}
