/**
 *  @package Ardukit
 *
 *  @module Button - Noise filteriing input button
 */

#include "platform.h"
#include "gtimer.h"
#include "gbutton.h"

using namespace adk;

const int SCAN_INTERVAL = 20;   // msec

//-----------------------------------------------------------------------------
//  class Button
//-----------------------------------------------------------------------------
Button::Button(int pin_id, int mode, int sensitivity)
    : m_pin_id(pin_id), m_mode(mode), m_sensitivity(sensitivity)
{
    if (m_sensitivity < 0) m_sensitivity = 0;
    if ((m_mode & 0x0FF) == INPUT_PULLUP) m_mode |= LOW_ON_PRESS;
}

Button::~Button()
{
    disable();
}

Button& Button::bind(int pin_id, int mode, int sensitivity)
{
    if (pin_id > 0) {
        m_pin_id = pin_id;
        m_mode = mode;
        m_sensitivity = (sensitivity < 0) ? 0 : sensitivity;
    }
    return *this;
}

void Button::enable()
{
    if ((m_mode & 0x00FF) == INPUT_PULLUP) pinMode(m_pin_id, INPUT_PULLUP);
    if (!m_timer_id) m_timer_id = set_timeout(scan, SCAN_INTERVAL, this);
}

void Button::disable()
{
    if (m_timer_id) {
        clear_timeout(m_timer_id);
        m_timer_id = 0;
    }
}

void Button::scan(void *data)
{
    Button *btn = (Button *)data;
    int delta = digitalRead(btn->m_pin_id) > 0 ? 1 : -1;
    if ((btn->m_mode & 0xFF00) == LOW_ON_PRESS) delta = -delta;

    int cur = btn->m_value + delta;
    if (cur > btn->m_sensitivity) cur = btn->m_sensitivity;
    if (cur < 0) cur = 0;
    btn->m_value = cur;

    if (!btn->m_is_pressed && cur >= btn->m_sensitivity) {
        btn->m_is_pressed = true;
        btn->emit("press");
    }
    if (btn->m_is_pressed && cur <= 0) {
        btn->m_is_pressed = false;
        btn->emit("release");
    }

    // dmsg("scan: sensitivity=%d, delta=%d cur==%d is_pressed=%d\n", btn->m_sensitivity, delta, cur, btn->is_pressed());
    if (btn->m_timer_id) set_timeout(scan, SCAN_INTERVAL, btn); // if enabled, set next scan schedule
}
