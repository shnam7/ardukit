/**
 *  @package Ardukit
 *
 *  @module gque - circular queue
 */

#include "adkdef.h"
#include "gque.h"
#include <string.h>

//--------------------------------------------------------------------
//	class adk::_gque_core
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//	class adk::_gque_core
//--------------------------------------------------------------------
adk::gque::~gque()
{
    if (m_q) delete[] m_q;
    m_q = 0;
}

void adk::gque::reset(unsigned capacity, unsigned itemSize) {
    if (gque::capacity() == capacity) return reset();
    if (capacity == 0) {
        if (m_q) delete [] m_q;
        m_q = 0;
        return;
    }
    unsigned bufSize = sizeof(gque_t) + (capacity + 1) * itemSize;
	m_q = (gque_t *)new char[bufSize];
    if (!m_q) {
        dmsg("_gque_core::_gque_core: memory allocation error.");
        return;
    }
    // init que
    m_q->item_size = itemSize;
	m_q->begin = sizeof(gque_t);
	m_q->end   = m_q->begin + itemSize*(capacity+1);
	m_q->head  = m_q->begin;
	m_q->tail  = m_q->begin;
    // dmsg("rst: b=%3d h=%3d t=%3d e=%3d size=%3d full=%d empty=%d len=%d avail=%d capa=%d\n",
    //      m_q->begin, m_q->head, m_q->tail, m_q->end, m_q->item_size, isFull(), isEmpty(),
    //      length(), available(), gque::capacity());
}

// append
bool adk::gque::put(const void *item) const
{
    if (!m_q) return false;
	unsigned npos = m_q->tail + m_q->item_size;
	if ( npos == m_q->end ) npos = m_q->begin;
	if ( npos == m_q->head ) return false;		/* full */
    memcpy(tail(), item, m_q->item_size);
    m_q->tail = npos;
	return true;
}

// pop from head
bool adk::gque::get(void *item) const
{
    if (!m_q || m_q->head == m_q->tail ) return false; /* empty */

    if (item) memcpy(item, head(), m_q->item_size);
	m_q->head += m_q->item_size;
	if ( m_q->head == m_q->end ) m_q->head = m_q->begin;
    return true;
}

// prepend
bool adk::gque::push(const void *item) const {
    if (!m_q) return false;
	unsigned pos = m_q->head - m_q->item_size;
	if ( pos < m_q->begin ) pos = m_q->end - m_q->item_size;
	if ( pos == m_q->head ) return false;		// full
    memcpy(_ptr(pos), item, m_q->item_size);
    m_q->head = pos;
	return true;
}

void *adk::gque::peekNext(const void *peek) const {
    if (!m_q) return 0;    // ensure there's more than one (next item)
    if (peek == 0) return head();   // return first entry
    if (peek < begin() || peek > end()) return 0;

    // dmsg("peek=%p q=%p b=%p h=%p t=%p e=%p\n", peek, m_q, begin(), head(), tail(), end());
    peek = (const char *)peek + m_q->item_size;
    if (peek == end()) peek = begin();
    if (peek == tail()) return 0;   // no more entries
    return (void *)peek;
}

bool adk::gque::isFull() const
{
    if (!m_q) return true;  // zero capacity
	unsigned npos = m_q->tail + m_q->item_size;
	if ( npos == m_q->end ) npos = m_q->begin;
	return npos == m_q->head;
}

unsigned adk::gque::length() const
{
    if (!m_q) return 0;
    unsigned n = (m_q->head <= m_q->tail)
        ? (m_q->tail - m_q->head)
        : (m_q->end - m_q->head) + (m_q->tail - m_q->begin);
    return n / m_q->item_size;
}

unsigned adk::gque::available() const
{
    if (!m_q) return 0;
	int n = ( m_q->head>m_q->tail ) ? (m_q->head-m_q->tail)
			: (m_q->end-m_q->tail) + (m_q->head-m_q->begin);
	return (n / m_q->item_size) - 1;
}

unsigned adk::gque::capacity() const
{
    if (!m_q) return 0;
    return ((m_q->end - m_q->begin) / m_q->item_size) - 1;
}
