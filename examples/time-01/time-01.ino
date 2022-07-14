#include "ardukit.h"

using namespace adk;

tick_t tm0;

void setup()
{
    Serial.begin(115200);
    tm0 = ticks();
}


void loop()
{
    unsigned long tm1_us = micros();
    tick_t tm1 = ticks();
    tick_t elapsed = elapsed_ticks(tm0);

    char s_tm0[64]; u64_to_str(s_tm0, tm0);
    char s_tm1[64]; u64_to_str(s_tm1, tm1);
    char s_elapsed[64]; u64_to_str(s_elapsed, elapsed);
    dmsg("sizeof(tick_t)=%d tm0=%s tm1_us=%lu tm1=%s elapsed=%s %lu(us) %lu(ms)\n",
        sizeof(tick_t), s_tm0, tm1_us, s_tm1, s_elapsed, t2u(elapsed), t2m(elapsed));
}
