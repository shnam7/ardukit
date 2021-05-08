/**
 *  @package Ardukit
 *
 *  @module gque - circular queue
 */

#pragma once
#include "adkdef.h"

namespace adk {

//--- generic cuicular queue
class circular_queue {
protected:
    typedef struct {
        unsigned begin;     // constant: start of que (end of que contro block)
        unsigned end;       // constant: end of que
        unsigned head;      // variable: que head
        unsigned tail;      // varibale: que tail
        unsigned item_size; // constant: entry item size
    } cque_t;

    cque_t          *m_q = 0;
    // template<class T> friend class que;

public:
    circular_queue() {}
    circular_queue(unsigned capacity, unsigned item_size) { init(capacity, item_size); }
    ~circular_queue();

	bool init(unsigned capacity, unsigned item_size);
    void clear() const { if (m_q) { m_q->head = m_q->tail = m_q->begin; } }

    bool put(const void *item=0) const;   // append
	bool get(void *item=0) const;       // pop from head
    bool push(const void *item=0) const;  // prepend
	bool pop(void *item=0) const;       // pop from tail

    void *first() const { return peek(); }
    void *last() const { return is_empty() ? 0 :
        _ptr((m_q->tail==m_q->begin ? m_q->end : m_q->tail) - m_q->item_size); }
    void *peek() const { return is_empty() ? 0 : _ptr(m_q->head); }
	void *peek_next(const void *current=0) const;
	void *peek_prev(const void *current=0) const;

	//--- accessors
	void *head() const { return m_q ? _ptr(m_q->head) : 0; }
	void *tail() const { return m_q ? _ptr(m_q->tail) : 0; }
	void *begin() const { return m_q ? _ptr(m_q->begin) : 0; }
	void *end() const { return m_q ? _ptr(m_q->end) : 0; }

	unsigned item_size() { return m_q ? m_q->item_size : 0; }

    bool is_empty() const { return !m_q || m_q->head==m_q->tail; }
    bool is_full() const;
	unsigned length() const;        // # of entries
    unsigned available() const;     // # of free space in item count
    unsigned capacity() const;

protected:
    void *_ptr(unsigned offs) const { return (void *)((char *)m_q + offs); }
};


//--- typed (circular) queue
template <class T>
class queue : public circular_queue {
public:
    queue() {}
    queue(unsigned capacity): circular_queue(capacity, sizeof(T)) {}

    bool init(unsigned capacity) { return circular_queue::init(capacity, sizeof(T)); };

	bool put(const T *item=0) const { return circular_queue::put(item); }
	bool get(T *item=0) const { return circular_queue::get(item); }
	bool push(const T *item=0) const { return circular_queue::push(item); }
	bool pop(T *item=0) const { return circular_queue::pop(item); }

    //--- support reference type interface
    bool put(const T &item) const { return put(&item); }
	bool get(T &item) const { return get(&item); }
	bool push(const T &item) const { return push(&item); }
	bool pop(T &item) const { return pop(&item); }

    T *first() const { return (T *)circular_queue::first(); }
    T *last() const { return (T *)circular_queue::last(); }
    T *peek() const { return (T *)circular_queue::peek(); }
	T *peek_next(const T *peek=0) const { return (T *)circular_queue::peek_next(peek); }
	T *peek_prev(const T *peek=0) const { return (T *)circular_queue::peek_prev(peek); }

	//--- accessors
	T *head() const { return (T *)circular_queue::head(); }
	T *tail() const { return (T *)circular_queue::tail(); }
	T *begin() const { return (T *)circular_queue::begin(); }
	T *end() const { return (T *)circular_queue::end(); }
};


} // namespace adk
