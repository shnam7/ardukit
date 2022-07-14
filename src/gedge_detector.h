/**
 *  @package Ardukit
 *
 *  @module EdgeDetector - Rising and falling edge detector for analog input
 *  @#notes
 *    - events: 'rising', 'falling'
 */

#pragma once

#include "gevent.h"

namespace adk {

//-----------------------------------------------------------------------------
// class EdgeDetector
//
// Emits 'rising' and 'falling' events
//-----------------------------------------------------------------------------
class EdgeDetector : public EventEmitter {
protected:
    float       m_low_threshould                = 0;
    float       m_high_threshould               = 0;
    float       (*m_input_reader)(void *data)   = 0;
    void        *m_input_reader_data            = 0;
    unsigned    m_timer_id                      = 0;
    bool        m_is_high                       = false;
    unsigned    m_scan_interval                 = 20;   // 20 msec by default

public:
    EdgeDetector(float low_threshould=5, float high_threshould=250, int max_listeners=5, unsigned scan_interval=20);
    ~EdgeDetector();

    EdgeDetector& set_threshoulds(float low_threshould, float high_threshould);
    EdgeDetector& set_input_reader(float (*input_reader)(void *data), void *data=0);
    virtual float read_input() { return m_input_reader ? m_input_reader(m_input_reader_data) : 0; }
    void set_scan_interval(unsigned interval) { m_scan_interval = interval; };
    unsigned scan_interval(unsigned interval) const { return m_scan_interval; };

    void start_input_scan();
    void stop_input_scan();

    bool is_high() const { return m_is_high; }
    bool is_low() const { return !m_is_high; }

    void detect_edge(float value);
    void detect_edge(int value) { EdgeDetector::detect_edge(static_cast<float>(value)); }

protected:
    static void scan(void *data);
};


//-----------------------------------------------------------------------------
//  class EdgeTrigger - Obsolete (To be removed in next release)
//-----------------------------------------------------------------------------
class EdgeTrigger : public EdgeDetector {
public:
    EdgeTrigger(int low_threshould=5, int high_threshould=250, int max_listeners=5)
        : EdgeDetector(static_cast<float>(low_threshould), static_cast<float>(high_threshould), max_listeners) {}
    ~EdgeTrigger() {}

    void set_threshoulds(int low_threshould, int high_threshould)
        { EdgeDetector::set_threshoulds(static_cast<float>(low_threshould), static_cast<float>(high_threshould)); }
};

}   // namespaec adk
