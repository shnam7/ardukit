/**
 *  @package Ardukit
 *
 *  @module GEdgeTrigger - Analog input edge detector
 */

#include "GEdgeTrigger.h"

//-----------------------------------------------------------------------------
//  class GEdgeErigger
//-----------------------------------------------------------------------------
void GEdgeTrigger::detect_edge(int value) {
    if (m_is_high) {
        if (value <= m_low_threshould) {
            m_is_high = false;
            emit("falling");
        }
    } else if (value >= m_high_threshould) {
        m_is_high = true;
        emit("rising");
    }
}
