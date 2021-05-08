#include "ardukit.h"

using namespace adk;

const int       TASK_COUNT = 5;
task            tasks[TASK_COUNT];
event_emitter   *evm[TASK_COUNT];

void event_handler(event_emitter::event &e)
{
    task &t = *(task *)e.data;
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

void task_func(task &t)
{
    t.sleep(1000);
}

void setup()
{
    Serial.begin(9600);

    for (int i = 0; i < TASK_COUNT; i++) {
        evm[i] = new event_emitter(3, 5);
        task &t = tasks[i];
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
