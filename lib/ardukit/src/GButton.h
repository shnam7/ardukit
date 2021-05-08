/**
 *  @package Ardukit
 *
 *  @module GButton - Noise filteriing input button
 *
 *  @notes
 *    - events: "press", "release"
 */

#pragma once
#include "gevent.h"

//-----------------------------------------------------------------------------
//  class GButton
//-----------------------------------------------------------------------------
class GButton : public adk::event_emitter {
protected:
    int         m_pin_id        = -1;
    int         m_value         = 0;
    int         m_io_type       = 0;
    int         m_sensitivity   = 2;
    bool        m_enabled       = false;
    bool        m_is_pressed    = false;

public:
    GButton(int pin_id=-1, int io_type=0, int sensitivity=2);
    ~GButton();

    void enable(int pin_id = -1);
    void disable() { m_enabled = false; }
    bool isPressed() { return m_is_pressed; }

protected:
    static void scan(void *data);
};
