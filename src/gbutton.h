/**
 *  @package Ardukit
 *
 *  @module Button - Noise filteriing input button
 *
 *  @notes
 *    - events: "press", "release"
 */

#pragma once
#include "gevent.h"

namespace adk {

//-----------------------------------------------------------------------------
//  class Button
//-----------------------------------------------------------------------------
class Button : public EventEmitter {
protected:
    int         m_pin_id        = -1;
    int         m_value         = 0;
    int         m_io_type       = INPUT;
    int         m_sensitivity   = 2;
    bool        m_enabled       = false;
    bool        m_is_pressed    = false;

public:
    Button(int pin_id=-1, int io_type=0, int sensitivity=2);
    ~Button();

    void enable(int pin_id = -1);
    void disable() { m_enabled = false; }
    bool is_pressed() { return m_is_pressed; }

protected:
    static void scan(void *data);
};

}   // namespaec adk
