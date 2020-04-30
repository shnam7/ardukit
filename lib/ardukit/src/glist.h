/**
 *  @package Ardukit
 *
 *  @module list - doubly linked list
 */

#pragma once
#include "adkdef.h"

namespace adk {

//-----------------------------------------------------------------------------
// doublely linked list
//-----------------------------------------------------------------------------
class glist {
public:
    class node {
    protected:
        node       *m_prev = this;
        node       *m_next = this;
        friend class glist;
        template<class T> friend class list;

    public:
        node() {}
        node(node *prev, node *next) : m_prev(prev), m_next(next) {}
        ~node() { detach(); }

        void append(node *nod) { nod->m_next = m_next; nod->m_prev = this; m_next->m_prev = nod; m_next = nod; }
        void prepend(node *nod) { m_prev->append(nod); }
        void detach() { m_prev->m_next = m_next; m_next->m_prev = m_prev; }
        bool isDetached() { return m_next == this; }
        unsigned length();  // count the length of the list it's atached
    };

protected:
    node           m_head;

public:
    glist() {}
    ~glist() { while (m_head.m_next != &m_head) del(m_head.m_next); }

    void add(node *nod) { append(nod); }
    void remove(node *nod) { nod->detach(); }
    void del(node *nod) { nod->detach(); delete nod; }

    void append(node *nod) { m_head.prepend(nod); }
    void prepend(node *nod) { m_head.append(nod); }

    node *first() { return m_head.m_next == &m_head ? 0 : m_head.m_next; }
    node *last() { return m_head.m_prev == &m_head ? 0 : m_head.m_prev; }
    node *nextOf(node *nod = 0)
        { if (!nod) { nod = &m_head; } return (nod->m_next == &m_head) ? 0 : nod->m_next; }

    unsigned length() { return m_head.length(); }
    bool isEmpty() { return m_head.isDetached(); }
};


template <class T = void>
class list : public glist {
public:
    list() {}
    ~list() {}

    void add(T *nod) { append(nod); }
    void remove(T *nod) { glist::remove(nod); }
    void del(T *nod) { glist::del(nod); }

    void append(T *nod) { glist::append(nod); }
    void prepend(T *nod){ glist::prepend(nod); }

    T *first() { return (T *)glist::first(); }
    T *last() { return (T *)glist::last(); }
    T *nextOf(T *nod = 0) { return (T *)glist::nextOf(nod); }
};

} // namespace adk


using GList = adk::glist;
