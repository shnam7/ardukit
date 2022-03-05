#include <ardukit.h>

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

adk::Button b1(buttonPin, INPUT_PULLUP);

void on_button_press(adk::EventEmitter::event &e)
{
    // turn LED on:
    digitalWrite(ledPin, HIGH);
};

void on_button_release(adk::EventEmitter::event &e)
{
    // turn LED off:
    digitalWrite(ledPin, LOW);
};

void setup()
{
    // initialize the LED pin as an output:
    pinMode(ledPin, OUTPUT);

    b1.enable();    // activate change detection
    b1.on("press", on_button_press);
    b1.on("release", on_button_release);
}

void loop() {
    adk::run();
}