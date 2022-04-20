/*
 *  @packaage Ardukit
 *
 *  @module timee
 */

#include "gtimer.h"
#include "gque.h"

using namespace adk;

const int MAX_CALL_STACK = 12;
struct eblock {
    void        (*func)(void *);
    void        *data;
    tick_t      interval;
    tick_t      next_run;
    bool        once;
    unsigned    id;
};

static unsigned         _counter = 0;
static Queue<eblock>    _eque(MAX_CALL_STACK);

void adk::clear_timeout(unsigned id)
{
    eblock *peek = _eque.peek();
    while (peek) {
        if (peek->id == id) { peek->func=0; break; }
        peek = _eque.peek_next(peek);
    }
}

unsigned timer_helpers::set_timer_block(void (*func)(void *), void *data, msec_t interval_msec, bool once)
{
    if (_eque.is_full()) return 0;  // invalid id
    _eque.put();    // make room at the end
    eblock *eb = _eque.last();

    eb->func = func;
    eb->data = data;
    eb->interval = msec_to_ticks(interval_msec);
    eb->next_run = ticks() + eb->interval;
    eb->once = once;
    eb->id = ++_counter;
    // dmsg(">set_timer: id=%d interval=%d next=%ld avail=%d\n", eb->id, interval_msec, eb->next_run, _eque.available());
    return eb->id;
}

void timer_helpers::run_timer()
{
    if (_eque.is_empty()) return;

    int len = _eque.length();
    tick_t tm = ticks();
    while (len-- > 0) {
        eblock *eb = _eque.first();
        _eque.get();

        if (eb->func && tm >= eb->next_run) {
            // dmsg(">run_timer: id=%d tm=%ld next=%ld interva=%d avail=%d\n", eb->id, tm, eb->next_run, eb->interval, _eque.available());
            eb->func(eb->data);
            if (eb->once)
                eb->func = 0;
            else
                eb->next_run = tm + eb->interval;
        }
        if (eb->func) _eque.put(eb);    // put back
    }
}
