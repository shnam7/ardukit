/**
 *  @package Ardukit
 *
 *  @module event - event emitter and listener
 */

#pragma once
#include "gmbox.h"
#include "glist.h"

#define MAX_EVENT_NAME_LENGTH  15

class GEvent {
protected:
    const char      *m_eventName;
    const void      *m_data;;
    unsigned long   m_extraData;

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
class GEventQ : public gcl::list<GEventQ>::node {
protected:
    typedef struct {
        GEvent::Handler     handler;        // event handler
        void                *data;          // user data
        unsigned long       extraData;      // extra data
        bool                once;
    } event_listener_t;

protected:
    char                            m_eventName[MAX_EVENT_NAME_LENGTH+1];
    gcl::mbox<event_listener_t>      m_evQ;
    friend class GEventEmitter;

public:
    GEventQ(const char *eventName, unsigned eventQSize=12);
    ~GEventQ() {}

    bool addListener(GEvent::Handler handler, void *data = 0,
                     unsigned long extraData = 0, bool once = false);
    void removeListener(GEvent::Handler handler);
    void processEvents();

    const char *eventName() { return m_eventName; }
    unsigned length() { return m_evQ.length(); }
};


//-----------------------------------------------------------------------------
//  class GEventEmitter
//-----------------------------------------------------------------------------
class GEventEmitter {
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
