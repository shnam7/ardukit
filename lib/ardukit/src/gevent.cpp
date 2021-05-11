/**
 *  @package Ardukit
 *
 *  @module event - event emitter and listener
 */

#include "gevent.h"
#include "gutil.h"
#include <string.h>

using namespace adk;

//-----------------------------------------------------------------------------
//  class GEventEmitter
//-----------------------------------------------------------------------------
bool EventEmitter::off(const char *event_name, event_listener listener)
{
    const char *name = __find_event_name(event_name);
    if (!name) return false;

    // erase relevant event listeners
    int len = m_listeners.length();
    int name_count = 0;
    while (len-- > 0) {
        event_block eb;
        m_listeners.get(eb);
        if (eb.name==name) {
            if (eb.listener==listener) continue;  // remove from the que
            name_count++;
        }
        m_listeners.put(eb);    // put back to the que
    }
    if (name_count == 0) __unregister_event_name(name);
    return true;
}

bool EventEmitter::emit(const char *event_name)
{
    const char *name = __find_event_name(event_name);
    if (!name) return false;

    event_block *peek = m_listeners.peek();
    while (peek) {
        if (peek->name == name) {
            if (peek->listener) peek->listener(*peek);     // call listener function
            if (peek->once) peek->name = 0;         // invalidate the entry
        }
        peek = m_listeners.peek_next(peek);    // put back to the que
    }
    return true;
}

bool EventEmitter::_on(const char *event_name, event_listener listener, void *data, bool once)
{
    if (strlen(event_name) > EVENT_NAME_LENGTH_MAX) {
        dmsg("EventEmitter::_on: event_name is too long. max length=%d\n", EVENT_NAME_LENGTH_MAX);
        return false;
    }

    const char *name = __register_event_name(event_name);
    if (!name) {
        dmsg("EventEmitter::_on: register_event_name on '%s' failed\n", event_name);
        return false;
    }

    if (m_listeners.available() <= 0) __squeez_listeners(); // try to make space
    if (m_listeners.available() <= 0) return false; // still no space, then fail

    event_block eb = { name, listener, data, once };
    return m_listeners.put(eb);
}

const char *EventEmitter::__find_event_name(const char *event_name)
{
    const char *peek = (const char *)m_names.peek();
    while (peek) {
        if (strcmp(peek, event_name) == 0) return peek;
        peek = (const char *)m_names.peek_next((event_name_t *)peek);
    }
    return 0;
}

const char *EventEmitter::__register_event_name(const char *event_name)
{
    const char *name = __find_event_name(event_name);
    if (name) return name;  // already registered

    // find empty slot first
    char *peek = (char *)m_names.peek();
    while (peek) {
        if (strlen(peek) == 0) {
            strncpy(peek, event_name, EVENT_NAME_LENGTH_MAX);
            peek[EVENT_NAME_LENGTH_MAX] = 0;
            return peek;
        }
        peek = (char *)m_names.peek_next((event_name_t *)peek);
    }

    // if no empty slot, then add to queue
    event_name_t entry;
    strncpy((char *)entry, event_name, EVENT_NAME_LENGTH_MAX);
    entry[EVENT_NAME_LENGTH_MAX] = 0;

    return m_names.put(entry) ? (const char *)m_names.last() : 0;
}

const char *EventEmitter::__unregister_event_name(const char *event_name)
{
    char *peek = (char *)m_names.peek();
    while (peek) {
        if (strcmp(peek, event_name) == 0) {
            peek[0] = 0;
            return peek;
        }
        peek = (char *)m_names.peek_next((event_name_t *)peek);
    }
    return 0;
}

void EventEmitter::__squeez_listeners()
{
    event_block *peek = m_listeners.peek();
    // trim from head
    while (peek && !peek->name) {
        m_listeners.get();
        peek = m_listeners.peek();
    }

    // trim from tail
    peek = m_listeners.last();
    while (peek && !peek->name) {
        m_listeners.pop();
        peek = m_listeners.last();
    }

    // trim inside
    int len = m_listeners.length();
    while (len-- > 0) {
        event_block eb;
        m_listeners.get(eb);
        if (!eb.name) continue; // trash the entry
        m_listeners.put(eb);    // put back to the que
    }
}
