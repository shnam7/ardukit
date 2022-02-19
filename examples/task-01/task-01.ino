#include "ardukit.h"

using namespace adk;

Task t1, t2, t3;

void task_func(void *)
{
    Task *t = Task::get_current();
    dmsg("Task %d is running. tick=%ld\n", t->task_id(), t2u(ticks()));
}

void setup()
{
    Serial.begin(128000);

    // init tasks
    t1.set_interval(1000).start(task_func); // periodic task w/ interval=1sec
    t2.set_interval(2000).start(task_func); // periodic task w/ interval=2sec
    t3.set_interval(3000).start(task_func); // periodic task w/ interval=3sec
}

void loop()
{
    adk::run();
}
