#include "ardukit.h"

using namespace adk;

void fade_restarter(event_emitter::event &e);

//--- LEDFader
class LEDFader : public task  {
protected:
    int             m_pinID = -1;
    int             m_brightness = 255;
    int             m_stepSize = 5;
    event_emitter   m_emitter;

public:
    LEDFader() : m_emitter(1, 2) { set_event_emitter(m_emitter); }
    void config(int pinID, int stepSize = 5, int interval = 10)
    {
        m_pinID = pinID;
        m_stepSize = stepSize;
        pinMode(m_pinID, OUTPUT);
        set_interval(interval);
    }

protected:
    virtual void run()
    {
        analogWrite(m_pinID, m_brightness);
        m_brightness -= m_stepSize;
        if (m_brightness < 0) {
            m_brightness = 255;
            suspend();
        }
    }
};


//--- Fade runner
const int LED_COUNT = 6;
const int LED_PINS[LED_COUNT] = {3, 5, 6, 9, 10, 11};
const int trailerSize = 200;
LEDFader faders[LED_COUNT];
bool isReverse = false;
int suspendCount = 0;

void fade_restarter(event_emitter::event &e)
{
    if (++suspendCount < LED_COUNT) return;

    int step = isReverse ? -trailerSize : trailerSize;
    int delay = isReverse ? trailerSize * LED_COUNT : 0;
    for (int i = 0; i < LED_COUNT; i++) {
        faders[i].resume(delay);
        delay += step;
    }
    isReverse = !isReverse;
    suspendCount = 0;
}

void setup()
{
    Serial.begin(128000);
    for (int i = 0; i < LED_COUNT; i++) {
        faders[i].config(LED_PINS[i]);
        faders[i].start(i*trailerSize);
        faders[i].on("suspend", fade_restarter);
    }
}

void loop()
{
    adk::run();
}
