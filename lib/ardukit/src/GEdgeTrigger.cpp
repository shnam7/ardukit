/**
 *  @package Ardukit
 *
 *  @module GEdgeTrigger - Analog input edge detector
 */

#include "GEdgeTrigger.h"

//-----------------------------------------------------------------------------
//  class GEdgeErigger
//-----------------------------------------------------------------------------
void GEdgeTrigger::triggerOnEdge(int value) {
    if (m_isHigh) {
        if (value <= m_low) {
            m_isHigh = false;
            emit("falling");
        }
    } else if (value >= m_high) {
        m_isHigh = true;
        emit("falling");
    }
}
