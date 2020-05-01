/**
 *  @package Ardukit
 *
 *  @module task - periodic tasks
 */

#include "gtask.h"
#include "gtimer.h"
#include "platform.h"

//-----------------------------------------------------------------------------
// GTask
//-----------------------------------------------------------------------------
unsigned GTask::s_taskCount = 0;

void GTask::run()
{
    if (m_func) m_func(*this);
}

GTask& GTask::bind(TaskFunc func, tick_t interval, void *data)
{
    if (m_state != _INIT) return *this;
    m_func = func;
    m_data = data;
    m_interval = interval * 1000;
    setState(_PREPARED);
    scheduleNext();
    adk::taskManager.add(this);
    return *this;
}

void GTask::start(tick_t delay)
{
    if (m_state == _INIT) {
        setState(_PREPARED);
        adk::taskManager.add(this);
    }
    if (m_state != _PREPARED) return;
    setState(_RUNNING);
    scheduleNext(delay);
}

void GTask::sleep(tick_t msec)
{
    if (m_state != _SLEEPING) setState(_SLEEPING);
    scheduleNext(msec);
}

void GTask::suspend()
{
    if (m_state == _SUSPENDED) return;
    setState(_SUSPENDED);
}

void GTask::awake(tick_t delay)
{
    if (m_state != _SLEEPING) return;
    setState(_RUNNING);
    scheduleNext(delay);
}

void GTask::resume(tick_t delay)
{
    if (m_state != _SUSPENDED) return;
    setState(_RUNNING);
    scheduleNext(delay);
}

void GTask::schedule(tick_t when_usec)
{
    m_nextTick = when_usec;
}

void GTask::scheduleNext(tick_t delay)
{
    delay = (delay==0) ? m_interval : delay * 1000;
    m_nextTick = GTimer::uticks() + delay;
}

void GTask::setState(unsigned state)
{
    if (m_state == state) return;

    m_lastState = m_state;
    m_state = state;

    switch (state) {
    case _PREPARED:
        onPrepare();
        emit("prepare");
        break;
    case _RUNNING:
        switch (m_lastState) {
            case _PREPARED: onStart(); emit("start"); break;
            case _SLEEPING: onAwake(); emit("awake"); break;
            case _SUSPENDED: onResume(); emit("resume"); break;
        }
        break;
    case _SLEEPING:
        onSleep();
        emit("sleep");
        break;
    case _SUSPENDED:
        onSuspend();
        emit("suspend");
        break;
    }
}



//-----------------------------------------------------------------------------
// GTaskManager
//-----------------------------------------------------------------------------
void GTaskManager::add(GTask *task) {
    if (!m_head) {
        m_head = task;
    } else {
        GTask *tail = m_head;
        while (tail->m_next) tail = tail->m_next;
        tail->m_next = task;
    }
    if (!m_cur) m_cur = m_head;
}

void GTaskManager::remove(GTask *task) {
    if (!m_head) return;

    GTask *pPrev = m_head;
    GTask *pCurr = m_head->m_next;
    while (pCurr) {
        if (pCurr == task) break;
        pPrev = pCurr;
        pCurr = pCurr->m_next;
    }
    if (pCurr) {
        pPrev->m_next = pCurr->m_next;
        pCurr->m_next = 0;
    }
}

void GTaskManager::run() {
    GTimer::processEvents();
    if (!m_cur) return;

    switch (m_cur->m_state) {
    case GTask::_RUNNING: {
        tick_t tm = GTimer::uticks();
        if (tm >= m_cur->m_nextTick) {
            // do next schedule first to make it availe in run()
            // In additon, run() may do some rescheduling
            m_cur->scheduleNext();
            m_cur->run();
        }
        break;
    };
    case GTask::_SLEEPING: {
        tick_t tm = GTimer::uticks();
        if (tm >= m_cur->m_nextTick) {
            m_cur->setState(GTask::_RUNNING);
            m_cur->scheduleNext();
        }
        break;
    }
    default:
        break;
    }
    m_cur = (m_cur->m_next) ? m_cur->m_next : m_head; // move on to next task
    // dmsg("s%d t=%ld n=%ld i=%ld",  m_cur->m_state, GTimer::uticks(), m_cur->m_nextTick, m_cur->m_interval);
}

GTask *GTaskManager::getTask(unsigned id) {
    GTask *pT = m_head;
    while (pT) {
        if (pT->m_ID == id) break;
        pT = pT->m_next;
    }
    return pT;
}

GTask *GTaskManager::nextOf(GTask *pCurrent) {
    if (!pCurrent) return m_head;
    return pCurrent->m_next;
}

unsigned GTaskManager::taskCount() {
    return GTask::s_taskCount;
}


namespace adk {
    GTaskManager taskManager;
}
