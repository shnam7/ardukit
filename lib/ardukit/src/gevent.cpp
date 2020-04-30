/**
 *  @package Ardukit
 *
 *  @module event - event emitter and listener
 */

#include "gevent.h"
#include "gutil.h"
#include <string.h>

//-----------------------------------------------------------------------------
//  class GEventQ
//-----------------------------------------------------------------------------
gcl_api gcl::GEventQ::GEventQ(const char *eventName, unsigned eventQSize) {
    strncpy(m_eventName, eventName, MAX_EVENT_NAME_LENGTH);
    m_eventName[MAX_EVENT_NAME_LENGTH] = 0;
    reset(eventQSize);
}

gcl_api bool gcl::GEventQ::addListener(GEvent::Handler handler, void *data,
                                  unsigned long extraData, bool once)
{
    _event_listener entry = {handler, data, extraData, once};
    return put(&entry);
}

gcl_api void gcl::GEventQ::removeListener(GEvent::Handler handler) {
    _lock();
    _event_listener *ev = (_event_listener *)gque::peek();
    void *tail_marker = gque::tail();
    while (ev) {
        GEvent event = { m_eventName, ev->data, ev->extraData };
        if (ev->handler == handler) gque::pop();
        ev = (ev == tail_marker) ? 0 : (_event_listener *)gque::peek();
    }
    _unlock();
}


gcl_api void gcl::GEventQ::processEvents() {
    _lock();
    _event_listener *ev = (_event_listener *)gque::peek();
    void *tail_marker = gque::tail();
    while (ev) {
        GEvent event = { m_eventName, ev->data, ev->extraData };
        ev->handler(event);
        if (!ev->once) gque::pop();
        ev = (ev == tail_marker) ? 0 : (_event_listener *)gque::peek();
    }
    _unlock();
}


//-----------------------------------------------------------------------------
//  class GEventEmitter
//-----------------------------------------------------------------------------
gcl_api void GEventEmitter::_on(const char *eventName, GEvent::Handler handler,
                        void *data, unsigned long extraData, bool once)
{
    GEventQ *eQ = _findEventQ(eventName);
    if (!eQ) {
        eQ = new GEventQ(eventName, m_eqSize);
        if (!eQ) dmsg("GEventEmitter::_on:memory allocation failure.");
        m_eqList.append(eQ);
    }
    eQ->addListener(handler, data, extraData, once);
}

gcl_api void GEventEmitter::off(const char *eventName, GEvent::Handler handler)
{
    GEventQ *evQ = _findEventQ(eventName);
    if (evQ) evQ->removeListener(handler);
}

gcl_api void GEventEmitter::emit(const char *eventName) {
    GEventQ *evQ = (GEventQ *)_findEventQ(eventName);
    if (evQ) evQ->processEvents();
}


gcl_api gcl::GEventQ *GEventEmitter::_findEventQ(const char *eventName) {
    GEventQ *evQ = m_eqList.first();
    while (evQ) {
        if (strcmp(evQ->eventName(), eventName) == 0) return evQ;
        evQ = m_eqList.nextOf(evQ);
    }
    return 0;
}
