#include "ardukit.h"

using namespace adk;

int brightness  = 0;
int delta       = 5;
tick_t tm0      = 0;
tick_t elapsed  = 0;

void set_LED(void *)
{
    tick_t tm1 = ticks();
    dmsg("LED brightness:%d elapsed=%d(msec)\n", brightness, ticks_to_msec(tm1-tm0));
    tm0 = tm1;

    brightness += delta;
    bool edge = brightness == 255 || brightness == 0;
    if (edge) delta = -delta;
    set_timeout(set_LED, edge ? 2000 : 0);
}

void setup()
{
    Serial.begin(115200);
    set_timeout(set_LED, 0);
}

void loop()
{
    adk::run();
}
