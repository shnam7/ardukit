/**
 *  @package Ardukit
 *
 *  @module timer and timer events
 */

#include "gtime.h"

// using event_listener = GEventQ::event_listener;

// static GQue _timerEventQ(GTimer::TIMER_EVENT_QUE_SIZE, sizeof(event_listener));

// handle_t GTimer::setTimeout(GEvent::Handler handler, tick_t msec, void *data)
// {
//     event_listener el = {handler, data, uticks() + msec * 1000, false};
//     return _timerEventQ.put(&el) ? (handle_t)handler : 0;
// }

// void GTimer::clearTimeout(handle_t handle)
// {
//     event_listener el;
//     unsigned len = _timerEventQ.length();
//     while (len-- > 0) {
//         _timerEventQ.get(&el);
//         if (el.handler != (GEvent::Handler)handle) _timerEventQ.put(&el);
//     }
// }

// void GTimer::processEvents()
// {
//     event_listener el;
//     unsigned len = _timerEventQ.length();
//     tick_t tm = GTimer::uticks();
//     while (len-- > 0) {
//         _timerEventQ.get(&el);
//         if (tm >= el.extraData) {
//             GEvent event = { "timer", el.data, el.extraData };
//             el.handler(event);
//             continue;
//         }
//         _timerEventQ.put(&el);
//     }
// };
