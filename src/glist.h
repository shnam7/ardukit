/**
 *  @package Ardukit
 *
 *  @module List - doubly linked list
 */

#pragma once

namespace adk {

//-----------------------------------------------------------------------------
// class Linkable
//-----------------------------------------------------------------------------
class Linkable {
protected:
    Linkable       *m_prev = this;
    Linkable       *m_next = this;

public:
    Linkable() {}
    ~Linkable() { detach(); }

    void append(Linkable *node);
    void prepend(Linkable *node) { node->append(this); }
    void append_to(Linkable *node) { node->append(this); }
    void prepend_to(Linkable *node) { node->prepend(this); }
    void detach() { m_prev->m_next = m_next; m_next->m_prev = m_prev; }

    void add(Linkable *node) { append(node); }
    void remove(Linkable *node) { node->detach(); }

    Linkable *next() const { return m_next; }
    Linkable *prev() const { return m_prev; }

    bool is_alone() const { return m_next == this; }
    unsigned length() const;  // count the length of the list excluding itself
};

template <class T = void>
class List : protected Linkable {
public:
    List() {}
    ~List() {}

    void append(T *node) { Linkable::append(node); };
    void prepend(T *node) { Linkable::prepend(node);; }
    void add(T *node) { Linkable::add(node); }
    void remove(T *node) { Linkable::remove(node); }

    T *first() { return is_empty() ? 0 : (T *)Linkable::next(); }
    T *last() { return is_empty() ? 0 : (T *)Linkable::prev(); }

    T *next_of(T *node = 0) {
        T *item = node ? (T *)node->next() : first();
        return item == (T *)this ? 0 : item;
    }

    T *prev_of(T *node = 0) {
        T *item = node ? (T *)node->prev() : last();
        return item == ( T *)this ? 0 : item;
    }

    unsigned is_empty() { return Linkable::is_alone(); }
    unsigned length() { return Linkable::length(); }

public:
    //--- Iterators
    class Iterator {
        Linkable       *m_p = 0;
    public:
        Iterator(Linkable *p) : m_p(p) {}
        ~Iterator() {}
        T& operator*() const { return *(T *)m_p; }
        T *operator->() { return (T *)m_p; }
        Iterator& operator++() { m_p = m_p->m_next; return *this; }
        Iterator operator++(int) { Iterator tmp(m_p); m_p=m_p->next(); return tmp; }
        Iterator& operator--() { m_p = m_p->m_prev; return *this; }
        Iterator operator--(int) { Iterator tmp(m_p); m_p=m_p->prev(); return tmp; }
        bool operator==(const Iterator& rval) const { return m_p == rval.m_p; }
        bool operator!=(const Iterator& rval) const { return m_p != rval.m_p; }
    };

    Iterator begin() { return Iterator((T *)this->m_next); }
    Iterator end() { return Iterator((T *)this); }
    Iterator rbegin() { return Iterator((T *)this->m_prev); }
    Iterator rend() { return Iterator((T *)this); }
};

} // namespace adk
