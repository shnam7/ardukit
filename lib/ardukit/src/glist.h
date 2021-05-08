/**
 *  @package Ardukit
 *
 *  @module list - doubly linked list
 */

#pragma once
#include "adkdef.h"

namespace adk {

//-----------------------------------------------------------------------------
// class linkable
//-----------------------------------------------------------------------------
class linkable {
protected:
    linkable       *m_prev = this;
    linkable       *m_next = this;

public:
    linkable() {}
    ~linkable() { detach(); }

    void append(linkable *node);
    void prepend(linkable *node) { node->append(this); }
    void append_to(linkable *node) { node->append(this); }
    void prepend_to(linkable *node) { node->prepend(this); }
    void detach() { m_prev->m_next = m_next; m_next->m_prev = m_prev; }

    void add(linkable *node) { append(node); }
    void remove(linkable *node) { node->detach(); }

    linkable *next() const { return m_next; }
    linkable *prev() const { return m_prev; }

    bool is_alone() const { return m_next == this; }
    unsigned length() const;  // count the length of the list excluding itself
};

template <class T = void>
class list : protected linkable {
public:
    list() {}
    ~list() {}

    void append(T *node) { linkable::append(node); };
    void prepend(T *node) { linkable::prepend(node);; }
    void add(T *node) { linkable::add(node); }
    void remove(T *node) { linkable::remove(node); }

    T *first() { return is_empty() ? 0 : (T *)linkable::next(); }
    T *last() { return is_empty() ? 0 : (T *)linkable::prev(); }

    T *next_of(T *node = 0) {
        T *item = node ? (T *)node->next() : first();
        return item == (T *)this ? 0 : item;
    }

    T *prev_of(T *node = 0) {
        T *item = node ? (T *)node->prev() : last();
        return item == ( T *)this ? 0 : item;
    }

    unsigned is_empty() { return linkable::is_alone(); }
    unsigned length() { return linkable::length(); }
};

} // namespace adk
