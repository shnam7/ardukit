#include "ardukit.h"

class Fader : public GTask {
protected:
    int m_pinID         = 0;
    int m_value         = 0;
    int m_delta         = 0;
    int m_pauseDelay    = 0;

public:
    Fader() {}
    void config(int pinID, int interval=0, int delta=1, int pauseDelay=1000)
    {
        m_pinID = pinID;
        m_delta = delta;
        m_pauseDelay = pauseDelay;
        setInterval(10);
        pinMode(pinID, OUTPUT);
    }

protected:
    virtual void run() {
        analogWrite(m_pinID, m_value);
        m_value += m_delta;
        if (m_value > 255) {
            m_value = 255;
            m_delta = -m_delta;
        } else if (m_value < 0) {
            m_value = 0;
            m_delta = -m_delta;
        }
        if (m_value == 255 || m_value == 0) {
            sleep(m_pauseDelay);
        }
    }
};

const int LED_COUNT = 6;
const int LED_PINS[LED_COUNT] = {3, 5, 6, 9, 10, 11};
Fader faders[LED_COUNT];

void setup() {
    Serial.begin(9600);

    for (int i=0; i<LED_COUNT; i++) {
        faders[i].config(LED_PINS[i]);
        faders[i].start(i * 200);
    }
}

void loop() {
    adk::run();
}
