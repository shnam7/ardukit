/*
 *  @packaage Ardukit
 *
 *  @module timee
 */

#pragma once
#include "adkdef.h"
#include "gtime.h"

#ifndef ADK_DEFAULT_TIMER_EVENT_QUEUE_SIZE
#define ADK_DEFAULT_TIMER_EVENT_QUEUE_SIZE           8
#endif

namespace adk
{

//--- internal helpef functions
namespace timer_helpers {
    unsigned set_timer_block(void (*func)(void *), void *data, msec_t interval_msec, bool once);
    void run_timer();
} // namespace timer_helpers


// return non-zero timer ID on success. return zero on failure (timer que is full)
inline unsigned set_timeout(void (*func)(void *), msec_t timeout_msec, void *data=0)
    { return timer_helpers::set_timer_block(func, data, timeout_msec, true); }

// return non-zero timer ID on success. return zero on failure (timer que is full)
inline unsigned set_interval(void (*func)(void *), msec_t interval_msec, void *data=0)
    { return timer_helpers::set_timer_block(func, data, interval_msec, false); }

void clear_timeout(unsigned id);

void set_timer_event_queue_size(int size);
unsigned int get_timer_event_queue_size();

} // namespace adk
