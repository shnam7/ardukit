
#include "ardukit.h"

const int LED_COUNT = 6;
const int LED_PINS[LED_COUNT] = {3, 5, 6, 9, 10, 11};

//--- LEDFader
class LEDFader : public GTask {
protected:
    int m_pinID;
    int m_brightness;
    int m_stepSize;
    bool m_isRunning;

protected:
    virtual void run() {
        if (!m_isRunning) return;

        analogWrite(m_pinID, m_brightness);
        m_brightness -= m_stepSize;
        if (m_brightness < 0) m_isRunning = false;
    }

public:
    LEDFader(int pin, int interval=10, int stepSize=5) : GTask(interval),
        m_pinID(pin), m_brightness(0), m_stepSize(stepSize), m_isRunning(false) {
    }

    void start(int initialDelay=0) {
        m_brightness = 255;
        m_isRunning = true;
        pause(initialDelay);
    }

    bool isRunning() { return m_isRunning; }
};


class FadeRunner : public GTaskManager {
protected:
    int m_size;
    int m_trailer;
    bool m_isReverse;
    bool m_isRunning;

public:
    FadeRunner(const int size, const int *pinID, int trailer=100):
        m_size(size), m_trailer(trailer), m_isReverse(false), m_isRunning(false) {
        Serial.begin(9600);
        for (int i=0; i<size; i++)
            add(new LEDFader(pinID[i]));
    }

    void start() {
        m_isRunning = true;
    }

    void onFinish() {
        int stepSize = m_isReverse ? -m_trailer : m_trailer;
        int initialDelay = m_isReverse ? m_trailer * m_size : 0;

        // dmsg("onFinish: reverse=%d, pauseTime=%d, stepSize%d", m_isReverse, initialDelay, stepSize);

        LEDFader *pTask = (LEDFader *)getNextTask();
        while (pTask) {
            // dmsg("t%d pause=%d", pTask, initialDelay);
            pTask->start(initialDelay);
            initialDelay += stepSize;
            pTask = (LEDFader *)getNextTask(pTask);
        }
        m_isRunning = true;
        m_isReverse = !m_isReverse;
    }

    void run() {
        GTaskManager::run();
        if (!m_isRunning) return;

        // check if faders are still running
        bool isFinished = true;
        LEDFader *pTask = (LEDFader *)getNextTask();
        while (pTask) {
            if (pTask->isRunning()) {
                isFinished = false;
                break;
            }
            pTask = (LEDFader *)getNextTask(pTask);
        }
        if (isFinished) {
            m_isRunning = false;
            onFinish();
        }
    }
};


// FadeRunner fader;
FadeRunner *fader;

void setup() {
    fader = new FadeRunner(LED_COUNT, LED_PINS, 100);
    fader->start();
}

void loop() {
    fader->run();
}
