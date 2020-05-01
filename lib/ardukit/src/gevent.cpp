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
GEventQ::GEventQ(const char *eventName, unsigned eventQSize) {
    strncpy(m_eventName, eventName, MAX_EVENT_NAME_LENGTH);
    m_eventName[MAX_EVENT_NAME_LENGTH] = 0;
    reset(eventQSize, sizeof(event_listener));
}

bool GEventQ::addListener(GEvent::Handler handler, void *data,
                                  unsigned long extraData, bool once)
{
    event_listener entry = {handler, data, extraData, once};
    return put(&entry);
}

void GEventQ::removeListener(GEvent::Handler handler) {
    event_listener el;
    unsigned len = gque::length();
    while (len-- > 0) {
        gque::get(&el);
        if (el.handler != handler) gque::put(&el);
    }
}


void GEventQ::processEvents(event_processor eventProcessor, unsigned long data)
{
    event_listener el;
    unsigned len = gque::length();
    while (len-- > 0) {
        gque::get(&el);
        GEvent event = { m_eventName, el.data, el.extraData };
        if (!eventProcessor || !eventProcessor(m_eventName, el, data)) el.handler(event);
        if (!el.once) gque::put(&el);       // if not handled, put it back to the que
    }
}


//-----------------------------------------------------------------------------
//  class GEventEmitter
//-----------------------------------------------------------------------------
void GEventEmitter::_on(const char *eventName, GEvent::Handler handler,
                        void *data, unsigned long extraData, bool once)
{
    GEventQ *eQ = findEventQ(eventName);
    if (!eQ) {
        eQ = new GEventQ(eventName, m_eqSize);
        if (!eQ) {
            dmsg("GEventEmitter::_on:memory allocation failure.");
            return;
        }
        m_eqList.append(eQ);
    }
    if (!eQ->addListener(handler, data, extraData, once))
        dmsg("[GEventEmitter::_on] Warning:Event registration failed: eventName=%s", eventName);
}

void GEventEmitter::off(const char *eventName, GEvent::Handler handler)
{
    GEventQ *evQ = findEventQ(eventName);
    if (evQ) evQ->removeListener(handler);
}

void GEventEmitter::emit(const char *eventName) {
    GEventQ *evQ = (GEventQ *)findEventQ(eventName);
    if (evQ) evQ->processEvents();
}


GEventQ *GEventEmitter::findEventQ(const char *eventName) {
    GEventQ *evQ = m_eqList.first();
    while (evQ) {
        if (strcmp(evQ->eventName(), eventName) == 0) return evQ;
        evQ = m_eqList.nextOf(evQ);
    }
    return 0;
}
