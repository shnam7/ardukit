/**
 *  @package Ardukit
 *
 *  @module GButton - Noise filteriing input button
 */

#include "platform.h"
#include "gtimer.h"
#include "GButton.h"

const int SCAN_INTERVAL = 20;   // msec

//-----------------------------------------------------------------------------
//  class GButton
//-----------------------------------------------------------------------------
GButton::GButton(int pinID, int ioType, int sensitivity)
    : m_pinID(pinID), m_ioType(ioType), m_sensitivity(sensitivity)
{
    if (m_sensitivity < 0) m_sensitivity = 0;
}

GButton::~GButton() {
}

void GButton::enable(int pinID) {
    if (pinID >= 0) m_pinID = pinID;
    if (m_ioType == INPUT_PULLUP) digitalWrite(m_pinID, INPUT_PULLUP);
    GTimer::setTimeout(scan, SCAN_INTERVAL, this);
    m_enabled = true;
}

void GButton::scan(GEvent &e) {
    GButton *pB = (GButton *)e.data();
    int delta = digitalRead(pB->m_pinID) > 0 ? 1 : -1;
    if (pB->m_ioType == INPUT_PULLUP) delta = -delta;

    // int preVal = pB->m_value;
    int curVal = pB->m_value + delta;
    if (curVal > pB->m_sensitivity) curVal = pB->m_sensitivity;
    if (curVal < 0) curVal = 0;
    pB->m_value = curVal;

    if (!pB->m_isPressed && curVal >= pB->m_sensitivity) {
        pB->m_isPressed = true;
        pB->emit("press");
    }
    if (pB->m_isPressed && curVal <= 0) {
        pB->m_isPressed = false;
        pB->emit("releas");
    }
    // dmsg("scan: interval=%d, cur==%d isPressed=%d", pB->m_sensitivity, curVal, pB->isPressed());
    if (pB->m_enabled) GTimer::setTimeout(scan, SCAN_INTERVAL, pB);
}
