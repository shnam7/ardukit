/**
 *  @package Ardukit
 *
 *  @module Task - periodic tasks
 */

#include "gtask.h"
#include "gtime.h"
#include "platform.h"

using namespace adk;

//-----------------------------------------------------------------------------
// class Task
//-----------------------------------------------------------------------------
unsigned Task::__task_count = 0;
Task    *Task::__head = 0;    // head of task list
Task    *Task::__cur = 0;     // pointer to current task

Task::Task(msec_t interval) : m_id(++__task_count), m_interval(interval)
{
    // add to tsask list
    if (!__head) {
        __head = this;
    } else {
        Task *tail = __head;
        while (tail->m_next) tail = tail->m_next;
        tail->m_next = this;
    }
    if (!__cur) __cur = __head;
}

Task::~Task()
{
    // remove from the task list
    if (__head) {
        Task *prev = __head;
        Task *curr = __head->m_next;
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

Task& Task::start(task_func func, void *data)
{
    if (m_state == _INIT) {
        m_func = func;
        m_data = data;
        m_next_run = 0;     // set initial delay to zero
        m_state = _PREPARE;
    }
    return *this;
}

Task& Task::start(msec_t delay_msec)
{
    if (m_state == _INIT) {
        m_next_run = msec_to_ticks(delay_msec); // set initial delay
        m_state = _PREPARE;
    }
    return *this;
}

Task& Task::sleep(msec_t msec)
{
    if (m_state != _SLEEPING) {
        schedule_next(msec);
        m_state = _SLEEPING;
    }
    return *this;
}

Task& Task::awake(msec_t delay_msec)
{
    if (m_state == _SLEEPING) {
        schedule_next(delay_msec);
        m_state = _RUNNING;
    }
    return *this;
}

Task& Task::suspend()
{
    if (m_state != _SUSPENDED) {
        // schedule_next();
        m_state = _SUSPENDED;
    }
    return *this;
}

Task& Task::resume(msec_t delay_msec)
{
    if (m_state == _SUSPENDED) {
        schedule_next(delay_msec);
        m_state = _RUNNING;
    }
    return *this;
}

void Task::schedule_next(msec_t delay_msec)
{
    m_next_run = ticks() +
        ((delay_msec==0) ? m_interval : delay_msec);
}

void Task::run()
{
    if (m_func) m_func(m_data);
}

void Task::_schedule() {
    switch (m_state) {
    case _PREPARE:
       if (m_state != m_last_state) {
            // on_prepare();
            if (m_emitter) m_emitter->emit("prepare");
            m_last_state = m_state;
            m_state = _RUNNING;
            m_next_run += ticks();  // add current tick to initial delay
        }
        break;
    case _RUNNING:
        if (m_state != m_last_state) {
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
            m_last_state = m_state;
        }
        else if (ticks() >= m_next_run) {
            // do next schedule first to make it availe in run()
            // In additon, run() may do some rescheduling
            schedule_next();
            run();
        }
        break;
    case _SLEEPING:
        if (m_state != m_last_state) {
            // on_sleep();
            if (m_emitter) m_emitter->emit("sleep");
            m_last_state = m_state;
        }
        else if (ticks() >= m_next_run) {
            schedule_next();
            m_state = _RUNNING;
        }
        break;
    case _SUSPENDED:
        if (m_state != m_last_state) {
            // on_suspend();
            if (m_emitter) m_emitter->emit("suspend");
            m_last_state = m_state;
        }
        break;
    }
}


void Task::schedule()
{
    if (!__cur) return;
    __cur->_schedule();
    __cur = (__cur->m_next) ? __cur->m_next : __head; // move on to next task
    // dmsg("tid=%d s%d t=%ld n=%ld i=%ld",  __cur->m_id, __cur->m_state, ticks(), __cur->m_next_run, __cur->m_interval);
}
