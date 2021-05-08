/**
 *  @package Ardukit
 *
 *  @module GEdgeTrigger - Analog input edge detector
 *  @#notes
 *    - events: 'rising', 'falling'
 */

#pragma once

#include "gevent.h"

//-----------------------------------------------------------------------------
//  class GEdgeTrigger
//-----------------------------------------------------------------------------
class GEdgeTrigger : public adk::event_emitter {
protected:
    int     m_low_threshould;
    int     m_high_threshould;
    bool    m_is_high = false;

public:
    GEdgeTrigger(int low_threshould=5, int high_threshould=250, int max_listeners=5)
        : event_emitter(2, max_listeners), m_low_threshould(low_threshould),
        m_high_threshould(high_threshould) {}
    ~GEdgeTrigger() {}

    void set_threshoulds(int low_threshould, int high_threshould)
        { m_low_threshould = low_threshould; m_high_threshould = high_threshould; m_is_high = false; }
    bool is_high() { return m_is_high; }

    void detect_edge(int value);
};
