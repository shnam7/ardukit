/**
 *  @package Ardukit
 *
 *  @module Button - Noise filteriing input button
 */

#include "platform.h"
#include "gtime.h"
#include "gtimer.h"
#include "gbutton.h"

using namespace adk;

const int SCAN_INTERVAL = 20;   // msec

//-----------------------------------------------------------------------------
//  class Button
//-----------------------------------------------------------------------------
Button::Button(int pin_id, int io_type, int sensitivity)
    : m_pin_id(pin_id), m_io_type(io_type), m_sensitivity(sensitivity)
{
    if (m_sensitivity < 0) m_sensitivity = 0;
}

Button::~Button() {
}

void Button::enable(int pin_id) {
    if (pin_id >= 0) m_pin_id = pin_id;
    if (m_io_type == INPUT_PULLUP) digitalWrite(m_pin_id, INPUT_PULLUP);
    set_timeout(scan, SCAN_INTERVAL, this);
    m_enabled = true;
}

void Button::scan(void *data) {
    Button *btn = (Button *)data;
    int delta = digitalRead(btn->m_pin_id) > 0 ? 1 : -1;
    if (btn->m_io_type == INPUT_PULLUP) delta = -delta;

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

    dmsg("scan: sensitiviey=%d, delta=%d cur==%d is_pressed=%d\n", btn->m_sensitivity, delta, cur, btn->is_pressed());
    if (btn->m_enabled) set_timeout(scan, SCAN_INTERVAL, btn);
}
