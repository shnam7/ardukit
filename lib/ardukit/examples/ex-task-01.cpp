#include "ardukit.h"

GTask t1, t2, t3;

void taskFunc(GTask &t)
{
    dmsg("Task %d is running. tick=%ld", t.taskID(), GTimer::uticks());
}

void setup()
{
    Serial.begin(128000);

    // init tasks
    t1.bind(taskFunc, 1000).start(); // periodic task w/ interval=1sec
    t2.bind(taskFunc, 2000).start(); // periodic task w/ interval=2sec
    t3.bind(taskFunc, 3000).start(); // periodic task w/ interval=3sec
}

void loop()
{
    adk::run();
}
