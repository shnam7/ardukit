
#include "ardukit.h"

int brightness = 0;
int delta = 1;

void setLED(GEvent &e) {
    dmsg("LED brightness: %d", brightness);
    brightness += delta;
    if (brightness == 255) delta = -1;
    if (brightness == 0) delta = 1;
    GTimer::setTimeout(setLED, 10);
}

void setup() {
    Serial.begin(128000);
    GTimer::setTimeout(setLED);
}

void loop() {
    adk::run();
}
