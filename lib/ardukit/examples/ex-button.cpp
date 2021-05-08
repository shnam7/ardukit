#include "ardukit.h"

void on_button_event(adk::EventEmitter::event &e)
{
    dmsg("Button Event: %s", e.name);
};

adk::Button b1(8, INPUT_PULLUP);

void setup()
{
    Serial.begin(128000);
    b1.enable();    // activate change detection
    b1.on("press", on_button_event);
    b1.on("releas", on_button_event);
}

void loop()
{
    adk::run();
}
