/**
 *  @package Ardukit
 *
 *  @module gque - circular queue
 */

#include "gque.h"
#include "gutil.h"
#include <string.h>

using namespace adk;

//--------------------------------------------------------------------
//	class CircularQueue
//--------------------------------------------------------------------
CircularQueue::~CircularQueue()
{
    if (m_q) delete [] m_q;
    m_q = 0;
}

bool CircularQueue::init(unsigned capacity, unsigned item_size)
{
    if (CircularQueue::capacity() == capacity) { clear(); return true; }

    if (capacity == 0) {
        if (m_q) delete [] m_q;
        m_q = 0;
        return true;
    }

    // check if this call is for re-sizing
    if (m_q) delete [] m_q;

    unsigned buf_size = sizeof(cque_t) + (capacity + 1) * item_size;
	m_q = (cque_t *)new char[buf_size];
    if (!m_q) {
        dmsg("CircularQueue::init: memory allocation error.");
        return false;
    }
    // init que
    m_q->item_size = item_size;
	m_q->begin = sizeof(cque_t);
	m_q->end   = m_q->begin + item_size * (capacity + 1);
	m_q->head  = m_q->begin;
	m_q->tail  = m_q->begin;
    // dmsg("init: b=%3d h=%3d t=%3d e=%3d size=%3d full=%d empty=%d len=%d avail=%d capa=%d\n",
    //      m_q->begin, m_q->head, m_q->tail, m_q->end, m_q->item_size, is_full(), is_empty(),
    //      length(), available(), capacity());
    return true;
}

// append
bool CircularQueue::put(const void *item) const
{
    if (!m_q) return false;
	unsigned npos = m_q->tail + m_q->item_size;
	if ( npos == m_q->end ) npos = m_q->begin;
	if ( npos == m_q->head ) return false;  // full
    if (item) memcpy(_ptr(m_q->tail), item, m_q->item_size);
    m_q->tail = npos;
	return true;
}

// pop from head
bool CircularQueue::get(void *item) const
{
    if (!m_q || m_q->head == m_q->tail ) return false;  // empty

    if (item) memcpy(item, _ptr(m_q->head), m_q->item_size);
	m_q->head += m_q->item_size;
	if ( m_q->head == m_q->end ) m_q->head = m_q->begin;
    return true;
}

// prepend
bool CircularQueue::push(const void *item) const
{
    if (!m_q) return false;
	unsigned pos = m_q->head - m_q->item_size;
	if ( pos < m_q->begin ) pos = m_q->end - m_q->item_size;
	if ( pos == m_q->head ) return false;		// full
    if (item) memcpy(_ptr(pos), item, m_q->item_size);
    m_q->head = pos;
	return true;
}

// pop from tail
bool CircularQueue::pop(void *item) const
{
    if (!m_q || m_q->head == m_q->tail ) return false;  // empty

    if (item) memcpy(item, _ptr(m_q->tail-m_q->item_size), m_q->item_size);
	m_q->tail -= m_q->item_size;
    return true;
}

void *CircularQueue::peek_next(const void *peek) const
{
    if (is_empty()) return 0;
    if (peek == 0) return _ptr(m_q->head);   // return first entry
    if (peek < _ptr(m_q->begin) || peek >= _ptr(m_q->end)) return 0;   // sanity check

    // dmsg("peek=%p q=%p b=%p h=%p t=%p e=%p\n", peek, m_q, begin(), head(), tail(), end());
    peek = (const char *)peek + m_q->item_size;
    if (peek == _ptr(m_q->end)) peek = _ptr(m_q->begin);
    if (peek == _ptr(m_q->tail)) return 0;   // no more entries
    return (void *)peek;
}

void *CircularQueue::peek_prev(const void *peek) const
{
    if (is_empty()) return 0;
    if (peek == 0) return _ptr(m_q->tail - m_q->item_size);             // return last entry
    if (peek < _ptr(m_q->begin) || peek >= _ptr(m_q->end)) return 0;    // sanity check

    if (peek == _ptr(m_q->head)) return 0;   // no more entries
    if (peek == _ptr(m_q->begin)) peek = _ptr(m_q->end);
    peek = (const char *)peek - m_q->item_size;
    return (void *)peek;
}

bool CircularQueue::is_full() const
{
    if (!m_q) return true;  // zero capacity
	unsigned npos = m_q->tail + m_q->item_size;
	if ( npos == m_q->end ) npos = m_q->begin;
	return npos == m_q->head;
}

unsigned CircularQueue::length() const
{
    if (!m_q) return 0;
    unsigned n = (m_q->head <= m_q->tail)
        ? (m_q->tail - m_q->head)
        : (m_q->end - m_q->head) + (m_q->tail - m_q->begin);
    return n / m_q->item_size;
}

unsigned CircularQueue::available() const
{
    if (!m_q) return 0;
	int n = ( m_q->head>m_q->tail ) ? (m_q->head-m_q->tail)
			: (m_q->end-m_q->tail) + (m_q->head-m_q->begin);
	return (n / m_q->item_size) - 1;
}

unsigned CircularQueue::capacity() const
{
    return m_q ? ((m_q->end - m_q->begin) / m_q->item_size) - 1 : 0;
}
