#include "ardukit.h"

int brightness = 0;
int delta = 5;
tick_t tm0 = 0;
tick_t elapsed = 0;

void setLED(GEvent &e) {
    tick_t tm1 = GTimer::uticks();
    dmsg("LED brightness: %d elapsed=%ld", brightness, tm1-tm0);
    tm0 = tm1;

    brightness += delta;
    bool edge = brightness == 255 || brightness == 0;
    if (edge) delta = -delta;
    GTimer::setTimeout(setLED, edge ? 2000 : 0);
}

void setup() {
    Serial.begin(128000);
    GTimer::setTimeout(setLED);
}

void loop() {
    adk::run();
}
