/**
 *  @package Ardukit
 *
 *  @module timer and timer events
 */

#include "gtimer.h"

//-----------------------------------------------------------------------------
//  GTimerEventQ
//-----------------------------------------------------------------------------
class GTimerEventQ : public adk::GEventQ {
public:
    GTimerEventQ() : GEventQ("timer", 32) {}

    void processEvents();
};

void GTimerEventQ::processEvents()
{
    _lock();
    adk::_event_listener *ev = (adk::_event_listener *)gque::peek();
    void *tail_marker = gque::tail();
    tick_t tm = GTimer::uticks();
    while (ev) {
        if (tm > ev->extraData) {
            GEvent event = { m_eventName, ev->data, ev->extraData };
            ev->handler(event);
            gque::pop();
        }
        ev = (ev == tail_marker) ? 0 : (adk::_event_listener *)gque::peek();
    }
    _unlock();
}

//-----------------------------------------------------------------------------
//  GTimer
//-----------------------------------------------------------------------------
static GTimerEventQ evQ;

handle_t GTimer::setTimeout(GEvent::Handler handler, tick_t msec, void *data)
{
    evQ.addListener(handler, data, uticks() + msec * 1000);
    return (handle_t)handler;
}

void GTimer::clearTimeout(handle_t handle)
{
    return evQ.removeListener((GEvent::Handler)handle);
}

void GTimer::processEvents()
{
    evQ.processEvents();
};
