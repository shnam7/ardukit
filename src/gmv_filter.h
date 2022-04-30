/**
 *  @package Ardukit
 *
 *  @module moving average filter
 */

#pragma once

#include "gque.h"


namespace adk {

//-----------------------------------------------------------------------------
// class MVFilter template
//-----------------------------------------------------------------------------
template <typename T>
class MVFilterT {
protected:
    Queue<T>    m_q;
    unsigned    m_size  = 0;
    unsigned    m_len   = 0;
    T           m_sum   = 0;

public:
    MVFilterT(unsigned size) :  m_q(size), m_size(size) {}

    ~MVFilterT() {}

    T filter(T val) {
        m_sum += val;
        if (m_len < m_size) {
            m_len++;
            m_q.put(val);
        }
        else {
            T x; m_q.get(&x);
            m_sum -= x;
            m_q.put(val);
        }
        return m_sum / m_len;
    }
};


//-----------------------------------------------------------------------------
// class MVFilter for double data type (default)
//-----------------------------------------------------------------------------
class MVFilter : public MVFilterT<double> {
public:
    MVFilter(unsigned size) : MVFilterT(size) {}
    ~MVFilter() {}

    double filter(double val) { return MVFilterT::filter(val); };
    double filter(long val) { return MVFilterT::filter(static_cast<double>(val)); };
};


} // namespace adk
