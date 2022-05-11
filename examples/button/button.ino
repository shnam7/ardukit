#include "ardukit.h"

using namespace adk;

void on_button_event(EventEmitter::event &e)
{
    Button *btn = (Button *)e.data;
    dmsg("Button Event: buttonID=%d %s\n", btn->pinID(), e.name);
}

Button b1(6, INPUT_PULLUP);
Button b2(7, Button::EXTERNAL_PULLDOWN);    // using external pull-up resistor
Button b3(8, Button::EXTERNAL_PULLUP);      // using external pull-up resistor
Button buttons[3];      // 3 INPUT_PULLUP buttons, not connected to any pins yet


void setup()
{
    Serial.begin(128000);

    b1.enable();
    b1.on("press", on_button_event, &b1);
    b1.on("release", on_button_event, &b1);

    b2.enable();
    b2.on("press", on_button_event, &b2);
    b2.on("release", on_button_event, &b2);

    b3.enable();
    b3.on("press", on_button_event, &b3);
    b3.on("release", on_button_event, &b3);

    // multiple button objects combined to pin 9~11 as INPUT_PULLUP
    Button *btn = buttons;
    for (int i=0; i<3; i++) {
        btn->bind(9+i, INPUT_PULLUP);
        btn->on("press", on_button_event, btn);
        btn->on("release", on_button_event, btn);
    }
}

void loop()
{
    run();
}
