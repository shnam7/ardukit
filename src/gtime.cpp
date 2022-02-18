/**
 *  @package Ardukit
 *
 *  @module timer and timer events
 */

#include "gtime.h"
#include <limits.h>

using namespace adk;

tick_t adk::ticks()
{
    static uint64_t         _curr_tick = 0; // (uint64_t)ULONG_MAX - 10000; // overflow test
    static unsigned long    mticks_last = 0;

    unsigned long mticks = micros();
    uint64_t delta = (mticks < mticks_last)
        ? ((uint64_t)ULONG_MAX - mticks_last) + mticks
        : mticks - mticks_last;

    mticks_last = mticks;
    _curr_tick += delta;

    return (tick_t)_curr_tick;
}
