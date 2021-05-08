/**
 *  @package Ardukit
 *
 *  @module main header
 */

#include "adkcore.h"
#include "gtimer.h"
#include "gtask.h"

using namespace adk;

void adk::run() {
    timer_helpers::run_timer();
    task::schedule();
}
