#include "ardukit.h"

using namespace adk;

int brightness = 0;
int delta = 5;

void set_LED(void *e)
{
    dmsg("LED brightness: %d", brightness);
    brightness += delta;
    if (brightness == 255 || brightness == 0) delta = -delta;
}

void start(void *)
{
    set_interval(set_LED, 10);
}

void setup()
{
    Serial.begin(9600);
    dmsg("Will start in 1 sec...");
    set_timeout(set_LED, 1000);
}

void loop()
{
    adk::run();
}
