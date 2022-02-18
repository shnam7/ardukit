#include "ardukit.h"

using namespace adk;

void fade_restarter(EventEmitter::event &e);

//--- LEDFader
class LEDFader : public Task  {
protected:
    int             m_pinID = -1;
    int             m_brightness = 255;
    int             m_stepSize = 5;

public:
    LEDFader() {}
    LEDFader& config(int pinID, int stepSize = 5, int interval = 10)
    {
        m_pinID = pinID;
        m_stepSize = stepSize;
        pinMode(m_pinID, OUTPUT);
        set_interval(interval);
        return *this;
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
const int   LED_COUNT = 6;
const int   LED_PINS[LED_COUNT] = {3, 5, 6, 9, 10, 11};
const int   trailerSize = 200;

bool            isReverse = false;
int             suspendCount = 0;
LEDFader        faders[LED_COUNT];
EventEmitter    evm(1, LED_COUNT*2);


void fade_restarter(EventEmitter::event &e)
{
    if (Task::get_current() != e.data) return;

    if (++suspendCount < LED_COUNT) return;

    int step = isReverse ? -trailerSize : trailerSize;
    int delay = isReverse ? trailerSize * LED_COUNT : 0;

    // LEDFader *faders = (LEDFader *)e.data;
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
        faders[i]
            .config(LED_PINS[i])
            .set_event_emitter(evm)
            .on("suspend", fade_restarter, faders+i)
            .start(i*trailerSize);
    }
}

void loop()
{
    adk::run();
}
