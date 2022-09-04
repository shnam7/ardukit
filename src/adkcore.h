/**
 *  @package Ardukit
 *
 *  @module main header
 */

#pragma once

namespace adk
{
    bool add_global_callback(void (*func)(void));

    // run ardukit engine: task queue and timer events
    void run();

}   // namespace adk
