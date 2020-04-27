/**
 *  @package Ardukit
 *
 *  @module timer and timer events
 */

#pragma once
#include "adkdef.h"
#include "platform.h"
#include "gevent.h"

//-----------------------------------------------------------------------------
//  GTimer
//-----------------------------------------------------------------------------
typedef unsigned long   tick_t;      // 32bit

namespace GTimer {
    // microsec counter
    inline tick_t ticks() { return millis(); }
    inline tick_t elapsed(tick_t since) { return ticks() - since; }

    inline tick_t uticks() { return micros(); }
    inline tick_t uElapsed(tick_t since) { return uticks() - since; }

    handle_t setTimeout(GEvent::Handler handler, tick_t msec=0, void *data=0);
    void clearTimeout(handle_t handle);
    void processEvents();   // process timer events
};
