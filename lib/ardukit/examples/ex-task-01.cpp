#include "ardukit.h"

using namespace adk;

task t1, t2, t3;

void task_func(task &t)
{
    dmsg("Task %d is running. tick=%ld", t.task_id(), ticks());
}

void setup()
{
    Serial.begin(12800);

    // init tasks
    t1.set_interval(1000).start(task_func); // periodic task w/ interval=1sec
    t2.set_interval(2000).start(task_func); // periodic task w/ interval=1sec
    t3.set_interval(3000).start(task_func); // periodic task w/ interval=1sec
}

void loop()
{
    adk::run();
}
