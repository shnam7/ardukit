/**
 *  @package Ardukit
 *
 *  @module EdgeDetector - Rising and falling edge detector for analog input
 */

#include "gedge_detector.h"
#include "gtimer.h"

using namespace adk;

//-----------------------------------------------------------------------------
//  class EdgeDetector
//-----------------------------------------------------------------------------
EdgeDetector::EdgeDetector(float low_threshould, float high_threshould, int max_listeners, unsigned scan_interval)
    : EventEmitter(2, max_listeners), m_low_threshould(low_threshould), m_high_threshould(high_threshould),
      m_scan_interval(scan_interval)
{
}

EdgeDetector::~EdgeDetector()
{
    stop_input_scan();
}

EdgeDetector& EdgeDetector::set_threshoulds(float low_threshould, float high_threshould)
{
    m_low_threshould = low_threshould;
    m_high_threshould = high_threshould;
    m_is_high = false;
    return *this;
}

EdgeDetector& EdgeDetector::set_input_reader(float (*input_reader)(void *data), void *data)
{
    m_input_reader = input_reader;
    m_input_reader_data = data;
    return *this;
}

void EdgeDetector::start_input_scan()
{
    if (!m_timer_id) m_timer_id = set_timeout(scan, m_scan_interval, this);
}

void EdgeDetector::stop_input_scan()
{
    if (m_timer_id) {
        clear_timeout(m_timer_id);
        m_timer_id = 0;
    }
}

void EdgeDetector::detect_edge(float value)
{
    if (m_is_high) {
        if (value <= m_low_threshould) {
            m_is_high = false;
            emit("falling");
        }
    }
    else if (value >= m_high_threshould) {
        m_is_high = true;
        emit("rising");
    }
}

void EdgeDetector::scan(void *data)
{
    EdgeDetector *inst = (EdgeDetector *)data;

    inst->detect_edge(inst->read_input());

    if (inst->m_timer_id) set_timeout(scan, inst->m_scan_interval, inst); // if enabled, set next scan schedule
}
