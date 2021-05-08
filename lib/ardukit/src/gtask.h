/**
 *  @package Ardukit
 *
 *  @module task - periodic tasks
 *  @notes
 *    - task events: "prepare", "start", "sleep", "awake", "suspend", "resume"
 */

#pragma once
#include "gtime.h"
#include "gevent.h"

#ifndef ADK_TASK_EVENT_QUE_SIZE
#define ADK_TASK_EVENT_QUE_SIZE     16
#endif


namespace adk {

//-----------------------------------------------------------------------------
//  class task
//-----------------------------------------------------------------------------
class task {
public:
    typedef void (*task_func)(task &t);

protected:
    enum { _INIT=0, _PREPARE, _RUNNING, _SLEEPING, _SUSPENDED };

    unsigned        m_id            = 0;
    unsigned        m_state         = _INIT;
    unsigned        m_last_state    = _INIT;
    usec_t    m_interval      = 0;
    usec_t    m_next_run      = 0;

    task_func       m_func          = 0;
    void            *m_data         = 0;
    event_emitter   *m_emitter      = 0;
    task            *m_next         = 0;        // link to next task

public:
    task(msec_t interval=0);
    ~task();

    task& set_interval(msec_t msec) { m_interval = msec; return *this; }
    task& set_event_emitter(event_emitter *emitter) { m_emitter = emitter; return *this; }
    task& set_event_emitter(event_emitter& emitter) { return set_event_emitter(&emitter); }

    task& start(task_func func, void *data=0);
    task& start(msec_t delay_msec = 0);   // initial delay in msec

    task& sleep(msec_t msec);
    task& awake(msec_t delay_msec=0);   // awake from sleep after 'delay' in msec
    task& suspend();
    task& resume(msec_t delay_msec=0);  // restart from suspend after 'delay' in msec

    bool is_active() { return m_state >= _RUNNING; }
    bool is_running() { return m_state == _RUNNING; }
    bool is_sleeping() { return m_state == _SLEEPING; }
    bool is_suspended() { return m_state == _SUSPENDED; }

    unsigned task_id() { return m_id; }
    msec_t interval() { return (msec_t)(m_interval); }    // return in msec
    void *data() { return m_data; }

    // //--- event emitter shariing I/F to reduce memory usage
    task& on(const char *event_name, event_emitter::event_listener listener, void *data=0)
        { if (m_emitter) m_emitter->on(event_name, listener, data, (u64_t)this); return *this; }

    task& off(const char *event_name, event_emitter::event_listener listener)
        { if (m_emitter) m_emitter->off(event_name, listener); return *this; }

    task& once(const char *event_name, event_emitter::event_listener listener, void *data=0)
        { if (m_emitter) m_emitter->once(event_name, listener, data, (u64_t)this); return *this; }

    task& emit(const char *event_name)
        { if (m_emitter) m_emitter->emit(event_name); return *this; }

    void schedule_next(msec_t delay_msec=0);   // set next execution time in milli sec

protected:
    virtual void run();
    void _set_state(unsigned state);

    //--- task management
    static unsigned     __task_count;
    static task         *__head;    // head of task list
    static task         *__cur;     // pointer to current task

public:
    static void schedule();
    static task *get_current() { return __cur; }
};

} // namespace adk
