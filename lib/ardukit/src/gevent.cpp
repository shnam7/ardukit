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
GEventQ::GEventQ(const char *eventName, unsigned eventQSize)
{
    strncpy(m_eventName, eventName, MAX_EVENT_NAME_LENGTH);
    m_eventName[MAX_EVENT_NAME_LENGTH] = 0;
    m_evQ.reset(eventQSize);
}

bool GEventQ::addListener(GEvent::Handler handler, void *data, unsigned long extraData, bool once)
{
    event_listener_t entry = {handler, data, extraData, once};
    return m_evQ.put(&entry);
}

void GEventQ::removeListener(GEvent::Handler handler)
{
    unsigned len = m_evQ.length();
    for (unsigned i = 0; i < len; i++) {
        event_listener_t evObj;
        m_evQ.get(&evObj);
        if (evObj.handler == handler) continue;
        m_evQ.put(&evObj); // put back again
    }
}

void GEventQ::processEvents()
{
    event_listener_t evObj;
    unsigned len = m_evQ.length();
    for (unsigned i = 0; i < len; i++) {
        m_evQ.get(&evObj);
        GEvent ev = {m_eventName, evObj.data, evObj.extraData};
        evObj.handler(ev);
        if (!evObj.once) m_evQ.put(&evObj); // put back again
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

void GEventEmitter::emit(const char *eventName)
{
    GEventQ *evQ = (GEventQ *)_findEventQ(eventName);
    if (evQ) evQ->processEvents();
}

GEventQ *GEventEmitter::_findEventQ(const char *eventName)
{
    GEventQ *evQ = m_eqList.first();
    while (evQ) {
        if (strcmp(evQ->eventName(), eventName) == 0) return evQ;
        evQ = m_eqList.nextOf(evQ);
    }
    return 0;
}
