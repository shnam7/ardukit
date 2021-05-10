/**
 *  @package Ardukit
 *
 *  @module timer and timer events
 */

#pragma once
#include "adkdef.h"
#include "platform.h"
// #include "gevent.h"

//-----------------------------------------------------------------------------
//  GTimer
//-----------------------------------------------------------------------------
// typedef unsigned long   tick_t;      // 32bit

// namespace GTimer {
//     const int TIMER_EVENT_QUE_SIZE  = 16;

//     // // microsec counter
//     // inline tick_t ticks() { return millis(); }
//     // inline tick_t elapsed(tick_t since) { return ticks() - since; }

//     // inline tick_t uticks() { return micros(); }
//     // inline tick_t uElapsed(tick_t since) { return uticks() - since; }

//     handle_t setTimeout(GEvent::Handler handler, tick_t msec=0, void *data=0);
//     void clearTimeout(handle_t handle);
//     void processEvents();   // process timer events
// };


namespace adk
{

typedef unsigned long tick_t;   // timer tick count type    (32-bits)
typedef unsigned long usec_t;   // microsec time type
typedef unsigned msec_t;   // millisec time type

//--- get time elapsed since system start in msec
inline tick_t ticks() { return micros(); }

inline usec_t ticks_to_usec(tick_t ticks) { return (usec_t)ticks; }
inline msec_t ticks_to_msec(tick_t ticks) { return (msec_t)(ticks_to_usec(ticks) / 1000); }

inline tick_t usec_to_ticks(usec_t usec) { return (tick_t)usec; }
inline msec_t usec_to_msec(usec_t usec) { return (msec_t)(usec / 1000); }

inline tick_t msec_to_ticks(msec_t msec) { return usec_to_ticks(((usec_t)msec) * 1000); }
inline usec_t msec_to_usec(msec_t msec) { return  ((usec_t)msec) * 1000; }

inline msec_t t2m(tick_t ticks) { return ticks_to_msec(ticks); }
inline usec_t t2u(tick_t ticks) { return ticks_to_usec(ticks); }
inline tick_t u2t(usec_t usec) { return usec_to_ticks(usec); }
inline msec_t u2m(usec_t usec) { return usec_to_msec(usec); }
inline tick_t m2t(msec_t msec) { return msec_to_ticks(msec); }
inline usec_t m2u(msec_t msec) { return msec_to_usec(msec); }

inline tick_t elapsed_ticks(tick_t tm) { return ticks() - tm; }
inline usec_t elapsed_usec(tick_t tm) { return ticks_to_usec(elapsed_ticks(tm)); }
inline msec_t elapsed_msec(tick_t tm) { return ticks_to_msec(elapsed_ticks(tm)); }

} // namespace gcl
