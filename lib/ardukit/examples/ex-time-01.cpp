#include "ardukit.h"

using namespace adk;

void setup() {
    Serial.begin(9600);
}

void loop() {
    tick_t tm0 = ticks();

    while (true) {
        usec_t usec_elapsed = elapsed_usec(tm0);
        msec_t msec_elapsed = elapsed_msec(tm0);

        dmsg("start=%ld elapsed=%d(us), %d(ms)\n",
            ticks_to_usec(tm0), usec_elapsed, msec_elapsed
        );
    }
}
