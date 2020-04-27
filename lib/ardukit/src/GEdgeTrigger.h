/**
 *  @package Ardukit
 *
 *  @module GEdgeTrigger - Analog input edge detector
 */

#pragma once

#include "gevent.h"

//-----------------------------------------------------------------------------
//  class GEdgeErigger
//  Events: 'rising', 'falling'
//-----------------------------------------------------------------------------
class GEdgeTrigger : public GEventEmitter {
protected:
    int     m_low;      // low threshould
    int     m_high;     // high threshould
    bool    m_isHigh;   // current level

public:
    GEdgeTrigger(int low=5, int high=250) : m_low(low), m_high(high), m_isHigh(false) {}
    ~GEdgeTrigger() {}

    void setThreshoulds(int low, int high) { m_low = low; m_high = high; m_isHigh = false; }
    bool isHigh() { return m_isHigh; }

    void triggerOnEdge(int value);
};
