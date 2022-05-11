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
public:
    static const int   HIGH_ON_PRESS        = 0;        // IO read returns HIGH on button press
    static const int   LOW_ON_PRESS         = 0x100;    // IO read returns LOW on button press
    static const int   EXTERNAL_PULLUP      = INPUT + LOW_ON_PRESS;
    static const int   EXTERNAL_PULLDOWN    = INPUT + HIGH_ON_PRESS;

public:
    int         m_pin_id        = -1;
    int         m_value         = 0;
    int         m_mode          = HIGH_ON_PRESS;
    int         m_sensitivity   = 2;
    bool        m_is_pressed    = false;
    unsigned    m_timer_id      = 0;

public:
    Button(int pin_id=-1, int mode=INPUT, int sensitivity=2);
    ~Button();

    Button& bind(int pin_id, int mode=INPUT, int sensitivity=2);
    void enable();  // start periodic input scanning
    void disable(); // disable periodic input scaning

    int pinID() const { return m_pin_id; }
    bool is_pressed() { return m_is_pressed; }

protected:
    static void scan(void *data);
};

}   // namespaec adk
