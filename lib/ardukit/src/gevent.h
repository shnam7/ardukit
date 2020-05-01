/**
 *  @package Ardukit
 *
 *  @module event - event emitter and listener
 */

#pragma once
#include "gque.h"
#include "glist.h"

#ifndef MAX_EVENT_NAME_LENGTH
#define MAX_EVENT_NAME_LENGTH   15
#endif

//-----------------------------------------------------------------------------
//  class GEvent
//-----------------------------------------------------------------------------
class GEvent {
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
class GEventQ : public adk::list<GEventQ>::node, public adk::gque {
public:
    typedef struct {
        GEvent::Handler     handler;        // event handler
        void                *data;          // user data
        unsigned long       extraData;      // extra data
        bool                once;
    } event_listener;
    typedef bool (*event_processor)(const char *eventName, event_listener& el, unsigned long data);

protected:
    char    m_eventName[MAX_EVENT_NAME_LENGTH+1];
    friend class GEventEmitter;

public:
    GEventQ(const char *eventName, unsigned eventQSize=16);

    bool addListener(GEvent::Handler handler, void *data = 0,
                     unsigned long extraData = 0, bool once = false);
    void removeListener(GEvent::Handler handler, unsigned long extraData=0);
    void processEvents(event_processor eventProcessor=0, unsigned long data=0);

    const char *eventName() { return m_eventName; }
};


//-----------------------------------------------------------------------------
//  class GEventEmitter
//-----------------------------------------------------------------------------
class GEventEmitter {
protected:
    adk::list<GEventQ>      m_eqList;      // event queue list
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

    GEventQ *findEventQ(const char *eventName);

protected:
    void _on(const char *eventName, GEvent::Handler handler, void *data = 0,
             unsigned long extraData = 0, bool once = false);
};
