/**
 *  @package Ardukit
 *
 *  @module task - periodic tasks
 */

#include "gtask.h"
#include "gtime.h"
#include "platform.h"

using namespace adk;

//-----------------------------------------------------------------------------
// class task
//-----------------------------------------------------------------------------
unsigned task::__task_count = 0;
task    *task::__head = 0;    // head of task list
task    *task::__cur = 0;     // pointer to current task

task::task(msec_t interval) : m_id(++__task_count), m_interval(interval)
{
    // add to tsask list
    if (!__head) {
        __head = this;
    } else {
        task *tail = __head;
        while (tail->m_next) tail = tail->m_next;
        tail->m_next = this;
    }
    if (!__cur) __cur = __head;
}

task::~task()
{
    // remove from the task list
    if (__head) {
        task *prev = __head;
        task *curr = __head->m_next;
        while (curr) {
            if (curr == this) break;
            prev = curr;
            curr = curr->m_next;
        }
        if (curr) {
            prev->m_next = curr->m_next;
            curr->m_next = 0;
        }
    }
    --__task_count;
}

task& task::start(task_func func, void *data)
{
    if (m_state == _INIT) _set_state(_PREPARE);
    if (m_state != _PREPARE) return *this;

    m_func = func;
    m_data = data;
    _set_state(_RUNNING);
    schedule_next();
    return *this;
}

task& task::start(msec_t delay_msec)
{
    if (m_state == _INIT) _set_state(_PREPARE);
    if (m_state != _PREPARE) return *this;

    _set_state(_RUNNING);
    schedule_next(delay_msec);
    return *this;
}

task& task::sleep(msec_t msec)
{
    if (m_state != _SLEEPING) _set_state(_SLEEPING);
    schedule_next(msec);
    return *this;
}

task& task::awake(msec_t delay_msec)
{
    if (m_state != _SLEEPING) return *this;
    _set_state(_RUNNING);
    schedule_next(delay_msec);
    return *this;
}

task& task::suspend()
{
    if (m_state == _SUSPENDED) return *this;
    _set_state(_SUSPENDED);
    return *this;
}

task& task::resume(msec_t delay_msec)
{
    if (m_state != _SUSPENDED) return *this;
    _set_state(_RUNNING);
    schedule_next(delay_msec);
    return *this;
}

void task::schedule_next(msec_t delay_msec)
{
    if (delay_msec==0) delay_msec = m_interval;
    m_next_run = ticks() + msec_to_ticks(delay_msec);
}

void task::run()
{
    if (m_func) m_func(*this);
}

void task::_set_state(unsigned state)
{
    if (m_state == state) return;

    m_last_state = m_state;
    m_state = state;

    switch (state) {
    case _PREPARE:
        // on_prepare();
        if (m_emitter) m_emitter->emit("prepare");
        break;
    case _RUNNING:
        switch (m_last_state) {
            case _PREPARE:
                // on_start();
                if (m_emitter) m_emitter->emit("start");
                break;
            case _SLEEPING:
                // on_awake();
                if (m_emitter) m_emitter->emit("awake");
                break;
            case _SUSPENDED:
                // on_resume();
                if (m_emitter) m_emitter->emit("resume");
                break;
        }
        break;
    case _SLEEPING:
        // on_sleep();
        if (m_emitter) m_emitter->emit("sleep");
        break;
    case _SUSPENDED:
        // on_suspend();
        if (m_emitter) m_emitter->emit("suspend");
        break;
    }
}

void task::schedule() {
    if (!__cur) return;

    switch (__cur->m_state) {
    case _RUNNING: {
        tick_t tm = ticks();
        if (tm >= __cur->m_next_run) {
            // do next schedule first to make it availe in run()
            // In additon, run() may do some rescheduling
            __cur->schedule_next();
            __cur->run();
        }
        break;
    };
    case _SLEEPING: {
        tick_t tm = ticks();
        if (tm >= __cur->m_next_run) {
            __cur->_set_state(_RUNNING);
            __cur->schedule_next();
        }
        break;
    }
    default:
        break;
    }
    __cur = (__cur->m_next) ? __cur->m_next : __head; // move on to next task
    // dmsg("tid=%d s%d t=%ld n=%ld i=%ld",  __cur->m_id, __cur->m_state, ticks(), __cur->m_next_run, __cur->m_interval);
}
