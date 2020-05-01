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
adk::GEventQ::GEventQ(const char *eventName, unsigned eventQSize) {
    strncpy(m_eventName, eventName, MAX_EVENT_NAME_LENGTH);
    m_eventName[MAX_EVENT_NAME_LENGTH] = 0;
    reset(eventQSize, sizeof(event_listener));
}

bool adk::GEventQ::addListener(GEvent::Handler handler, void *data,
                                  unsigned long extraData, bool once)
{
    event_listener entry = {handler, data, extraData, once};
    return put(&entry);
}

void adk::GEventQ::removeListener(GEvent::Handler handler) {
    event_listener el;
    unsigned len = gque::length();
    while (len-- > 0) {
        gque::get(&el);
        if (el.handler != handler) gque::put(&el);
    }
}


void adk::GEventQ::processEvents() {
    event_listener el;
    unsigned len = gque::length();
    while (len-- > 0) {
        gque::get(&el);
        GEvent event = { m_eventName, el.data, el.extraData };
        el.handler(event);
        if (!el.once) gque::put(&el);
    }
}


//-----------------------------------------------------------------------------
//  class GEventEmitter
//-----------------------------------------------------------------------------
void GEventEmitter::_on(const char *eventName, GEvent::Handler handler,
                        void *data, unsigned long extraData, bool once)
{
    GEventQ *eQ = _findEventQ(eventName);
    if (!eQ) {
        eQ = new GEventQ(eventName, m_eqSize);
        if (!eQ) {
            dmsg("GEventEmitter::_on:memory allocation failure.");
            return;
        }
        m_eqList.append(eQ);
    }
    eQ->addListener(handler, data, extraData, once);
}

void GEventEmitter::off(const char *eventName, GEvent::Handler handler)
{
    GEventQ *evQ = _findEventQ(eventName);
    if (evQ) evQ->removeListener(handler);
}

void GEventEmitter::emit(const char *eventName) {
    GEventQ *evQ = (GEventQ *)_findEventQ(eventName);
    if (evQ) evQ->processEvents();
}


adk::GEventQ *GEventEmitter::_findEventQ(const char *eventName) {
    GEventQ *evQ = m_eqList.first();
    while (evQ) {
        if (strcmp(evQ->eventName(), eventName) == 0) return evQ;
        evQ = m_eqList.nextOf(evQ);
    }
    return 0;
}
