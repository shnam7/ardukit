/*
 *  @packaage Ardukit
 *
 *  @module timee
 */

#pragma once
#include "adkdef.h"
#include "gtime.h"

namespace adk
{
    //--- internal helpef functions
    namespace timer_helpers {
        unsigned set_timer_block(void (*func)(void *), void *data, msec_t interval_msec, bool once);
        void run_timer();
    }

    inline unsigned set_timeout(void (*func)(void *), msec_t timeout_msec, void *data=0)
        { return timer_helpers::set_timer_block(func, data, timeout_msec, true); }

    inline unsigned set_interval(void (*func)(void *), msec_t interval_msec, void *data=0)
        { return timer_helpers::set_timer_block(func, data, interval_msec, false); }

    void clear_timeout(unsigned id);

} // namespace adk
