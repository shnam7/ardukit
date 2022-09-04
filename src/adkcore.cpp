/**
 *  @package Ardukit
 *
 *  @module main header
 */

#include "adkcore.h"

using namespace adk;

const int MAX_GLOBAL_CALLBACK   = 2;

static void (*__global_callbacks[MAX_GLOBAL_CALLBACK])(void) = {0, };


bool adk::add_global_callback(void (*func)(void)) {
    for (int i=0; i<MAX_GLOBAL_CALLBACK; i++) {
        if (!__global_callbacks[i]) {
            __global_callbacks[i] = func;
            return true;
        }
    }
    return false;
}

void adk::run() {
    for (int i=0; i<MAX_GLOBAL_CALLBACK; i++)
    if (__global_callbacks[i]) __global_callbacks[i]();

    // timer_helpers::run_timer();
    // Task::schedule();
}
