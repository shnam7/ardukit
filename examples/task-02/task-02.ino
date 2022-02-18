#include "ardukit.h"

using namespace adk;

const int       TASK_COUNT = 5;
Task            tasks[TASK_COUNT];

void event_handler(EventEmitter::event &e)
{
    Task &t = *(Task *)e.data;
    if (Task::get_current() != &t) return;

    unsigned task_id = t.task_id();
    tick_t tm = ticks_to_usec(ticks());

    if (strcmp(e.name, "start") == 0) {
        dmsg("Task %d started. time=%ld -----------\n", task_id, tm);
    }
    else if (strcmp(e.name, "sleep") == 0) {
        dmsg("Task %d is going to sleep. time=%ld\n", task_id, tm);
    }
    else if (strcmp(e.name, "awake") == 0) {
        dmsg("Task %d is waking up now. time=%ld\n", task_id, tm);
    }
}

void task_func(void *)
{
    Task *t = Task::get_current();
    // dmsg("task %d running...before sleep call\n", Task::get_current()->task_id());
    t->sleep(2000);  // task does not stop here - it just change the task state to sleep
    // dmsg("task %d running...after sleep call\n", Task::get_current()->task_id());
}

void setup()
{
    Serial.begin(128000);

    EventEmitter evm(3, 5*TASK_COUNT);
    for (int i = 0; i < TASK_COUNT; i++) {
        Task &t = tasks[i];
        t.set_interval((i + 1) * 1000)
            .set_event_emitter(evm)
            .on("start", event_handler, &t)
            .on("sleep", event_handler, &t)
            .on("awake", event_handler, &t)
            .start(task_func); // bind() returns GTask itself
    }
}

void loop()
{
    adk::run();
}
