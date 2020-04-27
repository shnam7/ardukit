/**
 *  @package Ardukit
 *
 *  @module GButton - Noise filteriing input button
 */

#pragma once
#include "gevent.h"

//-----------------------------------------------------------------------------
//  class GButton
//  events: "press", "releas"
//-----------------------------------------------------------------------------
class GButton : public GEventEmitter {
protected:
    int         m_pinID         = -1;
    int         m_value         = 0;
    int         m_ioType        = 0;
    int         m_sensitivity   = 2;
    bool        m_enabled       = false;
    bool        m_isPressed     = false;

public:
    GButton(int pinID=-1, int ioType=0, int sensitivity=2);
    ~GButton();

    void enable(int pinID = -1);
    void disable() { m_enabled = false; }
    bool isPressed() { return m_isPressed; }

protected:
    static void scan(GEvent &e);
};
