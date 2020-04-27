/**
 *  @package Ardukit
 *
 *  @module timer and timer events
 */

#include "gtimer.h"

//-----------------------------------------------------------------------------
//  GTimerEventQ
//-----------------------------------------------------------------------------
class GTimerEventQ : public GEventQ {
public:
    GTimerEventQ() : GEventQ("GTimer", 32) {}

    void processEvents();
};

void GTimerEventQ::processEvents()
{
    unsigned len = m_evQ.length();
    tick_t tick = GTimer::uticks();

    for (unsigned i = 0; i < len; i++) {
        event_listener_t evl;
        m_evQ.get(&evl);
        if (tick > evl.extraData) {
            GEvent ev = {m_eventName, evl.data, evl.extraData};
            evl.handler(ev);
            continue;
        }
        m_evQ.put(evl);
    }
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
