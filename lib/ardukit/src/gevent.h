/**
 *  @package Ardukit
 *
 *  @module event - event emitter and listener
 */

#pragma once
#include "gmbox.h"
#include "glist.h"

namespace gcl {

const int MAX_EVENT_NAME_LENGTH = 15;
class gcl_api GEvent {
protected:
    const char          *m_eventName;
    const void          *m_data;;
    unsigned long       m_extraData;

public:
    GEvent(const char *eventName, void *data, unsigned long extraData)
        : m_eventName(eventName), m_data(data), m_extraData(extraData) {}
    ~GEvent() {}

    const char *eventName() { return m_eventName; }
    const void *data() { return m_data; }
    unsigned long extraData() { return m_extraData; }

    typedef void (*Handler)(GEvent &e);
};


//-----------------------------------------------------------------------------
//  class GEventQ
//-----------------------------------------------------------------------------
typedef struct {
    GEvent::Handler     handler;        // event handler
    void                *data;          // user data
    unsigned long       extraData;      // extra data
    bool                once;
} _event_listener;

class gcl_api GEventQ : public gcl::list<GEventQ>::node, public gcl::mbox<_event_listener> {
protected:

protected:
    char                            m_eventName[MAX_EVENT_NAME_LENGTH+1];
    friend class GEventEmitter;

public:
    GEventQ(const char *eventName, unsigned eventQSize=32);

    bool addListener(GEvent::Handler handler, void *data = 0,
                     unsigned long extraData = 0, bool once = false);
    void removeListener(GEvent::Handler handler);
    void processEvents();

    const char *eventName() { return m_eventName; }
};


//-----------------------------------------------------------------------------
//  class GEventEmitter
//-----------------------------------------------------------------------------
class gcl_api GEventEmitter {
protected:
    gcl::list<GEventQ>      m_eqList;      // event queue list
    unsigned                m_eqSize = 12;

public:
    GEventEmitter(unsigned eventQSize=12): m_eqSize(eventQSize) {}
    ~GEventEmitter() {}

    void on(const char *eventName, GEvent::Handler handler, void *data=0, unsigned long extraData=0)
        { _on(eventName, handler, data, extraData); }

    void off(const char *eventName, GEvent::Handler handler);

    void once(const char *eventName, GEvent::Handler handler, void *data=0, unsigned long extraData=0)
        { _on(eventName, handler, data, extraData, true); }

    void emit(const char *eventName);

protected:
    void _on(const char *eventName, GEvent::Handler handler, void *data = 0,
             unsigned long extraData = 0, bool once = false);
    GEventQ *_findEventQ(const char *eventName);
};

} // namespace gcl

using GEvent = gcl::GEvent;
using GEventEmitter = gcl::GEventEmitter;
