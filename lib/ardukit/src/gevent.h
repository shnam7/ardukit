/**
 *  @package Ardukit
 *
 *  @module event - event emitter and listener
 */

#pragma once
#include "gque.h"
#include "glist.h"

namespace adk {

//-----------------------------------------------------------------------------
//  class EventEmitter
//-----------------------------------------------------------------------------
class EventEmitter {
public:
    static const int EVENT_NAME_LENGTH_MAX     = 15;

    struct                  event_block;
    typedef event_block     event;
    typedef void            (*event_listener)(event&);

    struct event_block {
        const char          *name;
        event_listener      listener;
        void                *data;
        u64_t               data_ex;        // extra data;
        bool                once;
    };


protected:
    // struct event_name { char name[EVENT_NAME_LENGTH_MAX + 1]; } event_name;
    typedef char event_name_t[EVENT_NAME_LENGTH_MAX+1];

    Queue<event_name_t>     m_names;
    Queue<event_block>      m_listeners;

public:
    EventEmitter(unsigned max_event_names=6, unsigned max_listeners=5)
        : m_names(max_event_names), m_listeners(max_listeners) {}
    ~EventEmitter() {}

    bool init(unsigned max_event_names=10, unsigned max_listeners=10)
        { return m_names.init(max_event_names) && m_listeners.init(max_listeners); }

    bool on(const char *event_name, event_listener listener, void *data=0, u64_t data_ex=0)
        { return _on(event_name, listener, data, data_ex); }

    bool off(const char *event_name, event_listener listener);

    bool once(const char *event_name, event_listener listener, void *data=0, u64_t data_ex=0)
        { return _on(event_name, listener, data, data_ex, true); }

    bool emit(const char *event_name);

protected:
    bool _on(const char *event_name, event_listener listener, void *data=0,
            u64_t data_ex=0, bool once=false);

    //--- helper function
    const char *__find_event_name(const char *event_name);
    const char *__register_event_name(const char *event_name);
    const char *__unregister_event_name(const char *event_name);
    void __squeez_listeners();
};

// using event = EventEmitter::event;

} // namespace adk
