/**
 *  @package Ardukit
 *
 *  @module EdgeTrigger - Analog input edge detector
 *  @#notes
 *    - events: 'rising', 'falling'
 */

#pragma once

#include "gevent.h"

namespace adk {

//-----------------------------------------------------------------------------
//  class EdgeTrigger
//-----------------------------------------------------------------------------
class EdgeTrigger : public EventEmitter {
protected:
    int     m_low_threshould;
    int     m_high_threshould;
    bool    m_is_high = false;

public:
    EdgeTrigger(int low_threshould=5, int high_threshould=250, int max_listeners=5)
        : EventEmitter(2, max_listeners), m_low_threshould(low_threshould),
        m_high_threshould(high_threshould) {}
    ~EdgeTrigger() {}

    void set_threshoulds(int low_threshould, int high_threshould)
        { m_low_threshould = low_threshould; m_high_threshould = high_threshould; m_is_high = false; }
    bool is_high() { return m_is_high; }

    void detect_edge(int value);
};

}   // namespaec adk
