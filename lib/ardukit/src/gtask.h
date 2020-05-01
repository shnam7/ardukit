/**
 *  @package Ardukit
 *
 *  @module task - periodic tasks
 */

#pragma once
#include "gtimer.h"
#include "gevent.h"

class GTask;
class GTaskManager;

//-----------------------------------------------------------------------------
//  GTask
//-----------------------------------------------------------------------------
class GTask : public GEventEmitter {
public:
    typedef void        (*TaskFunc)(GTask &t);
    friend class GTaskManager;

protected:
    enum { _INIT=0, _PREPARED, _RUNNING, _SLEEPING, _SUSPENDED };

    unsigned        m_ID            = ++GTask::s_taskCount;
    unsigned        m_state         = _INIT;
    unsigned        m_lastState     = _INIT;
    tick_t          m_interval      = 0;
    tick_t          m_nextTick      = 0;

    TaskFunc        m_func          = 0;
    void            *m_data         = 0;
    GTask           *m_next         = 0;

protected:
    virtual void onPrepare() {}
    virtual void onStart() {}
    virtual void run();
    virtual void onSleep() {}
    virtual void onAwake() {}
    virtual void onSuspend() {}
    virtual void onResume() {}

public:
    GTask() {}
    GTask(tick_t interval) : m_interval(interval*1000L) {}
    GTask(TaskFunc func, tick_t interval = 0) : m_interval(interval*1000), m_func(func) {}
    ~GTask() { --GTask::s_taskCount; }

    GTask& bind(TaskFunc func, tick_t interval=0, void *data=0);

    void start(tick_t delay = 0);   // initial delay in msec
    void sleep(tick_t msec);
    void suspend();

    void awake(tick_t delay=0);   // awake from sleep
    void resume(tick_t delay=0);  // restart from suspend after 'delay' msec

    void schedule(tick_t when_usec); // set next execution time in micros secs
    void scheduleNext(tick_t delay=0);     // set next execution time in milli sec

    bool isActive() { return m_state >= _RUNNING; }
    bool isSleeping() { return m_state == _SLEEPING; }
    bool isSuspended() { return m_state == _SUSPENDED; }

    void setInterval(tick_t msec) { m_interval = msec*1000L; }

    unsigned taskID() { return m_ID; }
    unsigned state() { return m_state; }
    unsigned lastState() { return m_lastState; }
    tick_t interval() { return m_interval/1000; }    // return in msec
    void *data() { return m_data; }

protected:
    void setState(unsigned state);

    //--- statics
    static unsigned s_taskCount;
};


//-----------------------------------------------------------------------------
// Task Manager
//-----------------------------------------------------------------------------
class GTaskManager {
protected:
    GTask       *m_head = 0;
    GTask       *m_cur  = 0;     // pointer to current task

public:
    GTaskManager() {}

    void add(GTask *task);
    void add(GTask &task) { add(&task); };
    void remove(GTask *task);
    void remove(GTask &task) { remove(&task); }

    void run();

    GTask *getTask(unsigned id);
    GTask *nextOf(GTask *pCurrent=0);

    unsigned taskCount();
};

namespace adk {
    extern GTaskManager taskManager;
    inline void run() { taskManager.run(); }
}
