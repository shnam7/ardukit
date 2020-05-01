#include "ardukit.h"

tick_t msecStart = GTimer::ticks();
tick_t usecStart = GTimer::uticks();

void setup() {
    Serial.begin(128000);
}

void loop() {
    // millisec
    tick_t msec = GTimer::ticks();
    tick_t msecElapsed = GTimer::elapsed(msecStart);

    // microsec
    tick_t usec = GTimer::uticks();
    tick_t usecElapsed = GTimer::uElapsed(usecStart);

    dmsg("msec=(start=%ld current=%ld, elapsed=%ld), usec=(start=%ld current=%ld elapsed=%ld)",
        msecStart, msec, msecElapsed, usecStart, usec, usecElapsed);
}
