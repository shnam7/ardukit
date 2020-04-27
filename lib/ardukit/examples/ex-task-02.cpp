
#include "ardukit.h"

GTask t1, t2;

bool pauseDone = false;


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

    // set up event listeners
    t1.on("start", eventHandler, &t1);
    t1.on("sleep", eventHandler, &t1);
    t1.on("awake", eventHandler, &t1);

    t2.on("start", eventHandler, &t2);
    t2.on("sleep", eventHandler, &t2);
    t2.on("awake", eventHandler, &t2);

    // now start tasks
    t1.bind(taskFunc, 1000).start();
    t2.bind(taskFunc, 2000).start();
}

void loop() {
    adk::run();
}
