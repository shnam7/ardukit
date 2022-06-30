#include "ardukit.h"

using namespace adk;

EdgeDetector sensor1;

float read_input(void *)
{
    return static_cast<float>(analogRead(A0)); // convert int to float
}

void on_rising(EventEmitter::event &)
{
    dmsg("Rising Edge detected\n");
}

void on_falling(EventEmitter::event &)
{
    dmsg("Falling Edge detected\n");
}

void setup()
{
    Serial.begin(115200);

    sensor1.set_threshoulds(700, 1000)
        .set_input_reader(read_input)
        .start_input_scan();
    sensor1.on("rising", on_rising);
    sensor1.on("falling", on_falling);
}

void loop()
{
    adk::run();
}
