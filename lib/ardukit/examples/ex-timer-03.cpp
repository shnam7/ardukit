
#include "ardukit.h"

int brightness = 0;
int delta = 1;
tick_t tm0 = 0;
tick_t elapsed = 0;

void setLED(GEvent &e) {
    tick_t tm1 = GTimer::uticks();
    dmsg("LED brightness: %d elapsed=%ld", brightness, tm1-tm0);
    tm0 = tm1;

    brightness += delta;
    if (brightness == 255) delta = -5;
    if (brightness == 0) delta = 5;

    bool pause = brightness == 255 || brightness == 0;
    GTimer::setTimeout(setLED, pause ? 3000 : 0);
}

void setup() {
    Serial.begin(128000);
    GTimer::setTimeout(setLED);
}

void loop() {
    adk::run();
}
