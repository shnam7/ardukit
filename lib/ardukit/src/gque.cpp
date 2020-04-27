/**
 *  @package Ardukit
 *
 *  @module gque - circular queue
 */

#include "adkdef.h"
#include "gque.h"
#include <string.h>

//--------------------------------------------------------------------
//	class gcl::_gque_core
//--------------------------------------------------------------------
// set item size=1 to avoid division by zero
static gcl::gque_t _empty_q = {
    sizeof(gcl::gque_t),
    sizeof(gcl::gque_t) + 1,
    sizeof(gcl::gque_t),
    sizeof(gcl::gque_t), 1};

gcl::gque::gque() : m_q(&_empty_q) {}

gcl::gque::gque(unsigned capacity, unsigned itemSize) : m_q(&_empty_q)
{
    reset(capacity, itemSize);
}

gcl::gque::~gque()
{
    if (m_q != &_empty_q) delete[] m_q;
    m_q = 0;
}

void gcl::gque::reset(unsigned capacity, unsigned itemSize) {
    if (gque::capacity() == capacity) return reset();
    if (capacity == 0) {
        if (m_q != &_empty_q) delete [] m_q;
        m_q = &_empty_q;
        return;
    }

    unsigned bufSize = sizeof(gque_t) + (capacity + 1) * itemSize;
	m_q = (gque_t *)new char[bufSize];
    if (!m_q) {
        m_q = &_empty_q;
        dmsg("_gque_core::_gque_core: memory allocation error.");
        return;
    }

    // init que
    m_q->item_size = itemSize;
	m_q->begin = sizeof(gque_t);
	m_q->end   = m_q->begin + itemSize*(capacity+1);
	m_q->head  = m_q->begin;
	m_q->tail  = m_q->begin;
}

// append
bool gcl::gque::put(const void *item) const
{
    // dmsg("put: b=%d h=%d t=%d e=%d size=%d\n", m_q->begin, m_q->head, m_q->tail, m_q->end, m_q->item_size);
	unsigned npos = m_q->tail + m_q->item_size;
	if ( npos == m_q->end ) npos = m_q->begin;
	if ( npos == m_q->head ) return false;		/* full */
    memcpy(tail(), item, m_q->item_size);
    m_q->tail = npos;
	return true;
}

// prepend
bool gcl::gque::push(const void *item) const {
	unsigned npos = m_q->head - m_q->item_size;
	if ( npos < m_q->begin ) npos = m_q->end - m_q->item_size;
	if ( npos == m_q->head ) return false;		// full
    memcpy(tail(), item, m_q->item_size);
    m_q->tail = npos;
	return true;
}

// pop from head
bool gcl::gque::pop(void *item) const
{
    if ( m_q->head == m_q->tail ) return false; /* empty */

    if (item) memcpy(item, head(), m_q->item_size);
	m_q->head += m_q->item_size;
	if ( m_q->head == m_q->end ) m_q->head = m_q->begin;
    return true;
}

void *gcl::gque::peekNext(const void *peek) const {
    if (isEmpty()) return 0;
    if (peek == 0) return head();   // return first entry
    void *next = (char *)peek + m_q->item_size;
    if (next == end()) next = begin();
    if (next == tail()) return 0;                 // reached end of que
    if (next < begin() || next > end()) return 0; // invalid pointer (not in que)
    return next;
}

bool gcl::gque::isFull() const
{
	unsigned npos = m_q->tail + m_q->item_size;
	if ( npos == m_q->end ) npos = m_q->begin;
	return npos == m_q->head;
}

unsigned gcl::gque::length() const
{
	unsigned n = ( m_q->head<=m_q->tail ) ? (m_q->tail-m_q->head)
			: (m_q->end-m_q->head) + (m_q->tail-m_q->begin);
	return n / m_q->item_size;
}

unsigned gcl::gque::available() const
{
	int n = ( m_q->head>m_q->tail ) ? (m_q->head-m_q->tail)
			: (m_q->end-m_q->tail) + (m_q->head-m_q->begin);
	return (n / m_q->item_size) - 1;
}

unsigned gcl::gque::capacity() const
{
    return ((m_q->end - m_q->begin) / m_q->item_size) - 1;
}
