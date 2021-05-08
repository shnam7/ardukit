#include "ardukit.h"

using namespace adk;

const int       TASK_COUNT = 5;
Task            tasks[TASK_COUNT];
EventEmitter   *evm[TASK_COUNT];

void event_handler(EventEmitter::event &e)
{
    Task &t = *(Task *)e.data;
    unsigned task_id = t.task_id();
    tick_t tm = ticks();

    if (strcmp(e.name, "start") == 0) {
        dmsg("Task %d started. time=%ld -----------", task_id, tm);
    }
    else if (strcmp(e.name, "sleep") == 0) {
        dmsg("Task %d is going to sleep. time=%ld", task_id, tm);
    }
    else if (strcmp(e.name, "awake") == 0) {
        dmsg("Task %d is waking up now. time=%ld", task_id, tm);
    }
}

void task_func(Task &t)
{
    t.sleep(1000);
}

void setup()
{
    Serial.begin(9600);

    for (int i = 0; i < TASK_COUNT; i++) {
        evm[i] = new EventEmitter(3, 5);
        Task &t = tasks[i];
        t.set_interval((i + 1) * 1000)
            .set_event_emitter(evm[i])
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
