#include "ardukit.h"

const int TASK_COUNT = 2;
GTask tasks[TASK_COUNT];

void eventHandler(GEvent &e) {
    GTask &task = *(GTask *)e.data();
    unsigned taskID = task.taskID();
    tick_t tm = GTimer::uticks();

    if (strcmp(e.eventName(),"start")==0) {
        dmsg("Task %d started. time=%ld -----------", taskID, tm);
    }
    else if (strcmp(e.eventName(),"sleep")==0) {
        dmsg("Task %d is going to sleep. time=%ld", taskID, tm);
    }
    else if (strcmp(e.eventName(),"awake")==0) {
        dmsg("Task %d is waking up now. time=%ld", taskID, tm);
    }
}

void taskFunc(GTask &t) {
    t.sleep(1000);
}

void setup() {
    Serial.begin(128000);

    for (int i=0; i<TASK_COUNT; i++) {
        GTask &t = tasks[i];
        t.on("start", eventHandler, &t);
        t.on("sleep", eventHandler, &t);
        t.on("awake", eventHandler, &t);
        t.bind(taskFunc, (i+1) * 1000).start();    // bind() returns GTask itself
    }
}

void loop() {
    adk::run();
}
