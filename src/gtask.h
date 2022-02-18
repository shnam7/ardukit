/**
 *  @package Ardukit
 *
 *  @module Task - periodic tasks
 *  @notes
 *    - Task events: "prepare", "start", "sleep", "awake", "suspend", "resume"
 */

#pragma once
#include "gtime.h"
#include "gevent.h"

namespace adk {

//-----------------------------------------------------------------------------
//  class Task
//-----------------------------------------------------------------------------
class Task {
public:
    typedef void (*task_func)(void *);

protected:
    enum { _INIT=0, _PREPARE, _RUNNING, _SLEEPING, _SUSPENDED };

    unsigned        m_id            = 0;
    unsigned        m_state         = _INIT;
    unsigned        m_last_state    = _INIT;
    tick_t          m_interval      = 0;
    tick_t          m_next_run      = 0;

    task_func       m_func          = 0;
    void            *m_data         = 0;
    EventEmitter    *m_emitter      = 0;
    Task            *m_next         = 0;        // link to next task

public:
    Task(msec_t interval=0);
    ~Task();

    Task& set_interval(msec_t msec) { m_interval = msec_to_ticks(msec); return *this; }
    Task& set_event_emitter(EventEmitter *emitter) { m_emitter = emitter; return *this; }
    Task& set_event_emitter(EventEmitter& emitter) { return set_event_emitter(&emitter); }

    Task& start(task_func func, void *data=0);
    Task& start(msec_t delay_msec = 0);   // initial delay in msec

    Task& sleep(msec_t msec);
    Task& awake(msec_t delay_msec=0);   // awake from sleep after 'delay' in msec
    Task& suspend();
    Task& resume(msec_t delay_msec=0);  // restart from suspend after 'delay' in msec

    bool is_active() { return m_state >= _RUNNING; }
    bool is_running() { return m_state == _RUNNING; }
    bool is_sleeping() { return m_state == _SLEEPING; }
    bool is_suspended() { return m_state == _SUSPENDED; }

    unsigned task_id() { return m_id; }
    msec_t interval() { return ticks_to_msec(m_interval); }    // return in msec
    void *data() { return m_data; }

    // //--- event emitter shariing I/F to reduce memory usage
    Task& on(const char *event_name, EventEmitter::event_listener listener, void *data=0)
        { if (m_emitter) m_emitter->on(event_name, listener, data); return *this; }

    Task& off(const char *event_name, EventEmitter::event_listener listener)
        { if (m_emitter) m_emitter->off(event_name, listener); return *this; }

    Task& once(const char *event_name, EventEmitter::event_listener listener, void *data=0)
        { if (m_emitter) m_emitter->once(event_name, listener, data); return *this; }

    Task& emit(const char *event_name)
        { if (m_emitter) m_emitter->emit(event_name); return *this; }

    void schedule_next(msec_t delay_msec=0);    // set next execution time in milli sec

protected:
    virtual void run();
    void _schedule();

    //--- task management
    static unsigned     __task_count;
    static Task         *__head;    // head of task list
    static Task         *__cur;     // pointer to current task

public:
    static void schedule();
    static Task *get_current() { return __cur; }
};

} // namespace adk
