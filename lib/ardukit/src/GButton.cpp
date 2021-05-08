/**
 *  @package Ardukit
 *
 *  @module GButton - Noise filteriing input button
 */

#include "platform.h"
#include "gtime.h"
#include "gtimer.h"
#include "GButton.h"

const int SCAN_INTERVAL = 20;   // msec

//-----------------------------------------------------------------------------
//  class GButton
//-----------------------------------------------------------------------------
GButton::GButton(int pin_id, int io_type, int sensitivity)
    : m_pin_id(pin_id), m_io_type(io_type), m_sensitivity(sensitivity)
{
    if (m_sensitivity < 0) m_sensitivity = 0;
}

GButton::~GButton() {
}

void GButton::enable(int pin_id) {
    if (pin_id >= 0) m_pin_id = pin_id;
    if (m_io_type == INPUT_PULLUP) digitalWrite(m_pin_id, INPUT_PULLUP);
    adk::set_timeout(scan, SCAN_INTERVAL, this);
    m_enabled = true;
}

void GButton::scan(void *data) {
    GButton *btn = (GButton *)data;
    int delta = digitalRead(btn->m_pin_id) > 0 ? 1 : -1;
    if (btn->m_io_type == INPUT_PULLUP) delta = -delta;

    // int preVal = btn->m_value;
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
        btn->emit("releas");
    }
    // dmsg("scan: interval=%d, cur==%d isPressed=%d", btn->m_sensitivity, cur, btn->isPressed());
    if (btn->m_enabled) adk::set_timeout(scan, SCAN_INTERVAL, btn);
}
