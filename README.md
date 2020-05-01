# Ardukit
Ardukit is a foundation library toolkit for easier arduino programming.

## Modules included:
- gtask: Periodic task manaer
- gevents: Event and EventEmitter
- glist: simple doubly linked list
- gque: sime circular queue
- gtimer: timer ticks and timer event services
- gutil: debuging functions, etc.
- GButton: Noise filteriing button input handler
- GEdgeTrigger: Noise filtering analog input up/down edge detector


## Quick Examples

#### Easy non-preemptive multi-tasking.
```cpp
GTask t1, t2, t3;

void taskFunc(GTask &t) {
    dmsg("Task %d is running. tick=%ld", t.taskID(), GTimer::uticks());
}

void setup() {
    Serial.begin(128000);

    // init tasks
    t1.bind(taskFunc, 1000).start();    // periodic task w/ interval=1sec
    t2.bind(taskFunc, 2000).start();    // periodic task w/ interval=2sec
    t3.bind(taskFunc, 3000).start();    // periodic task w/ interval=3sec
}

void loop() {
    adk::run();
}
```

GTask has an event emitter embedded, and emitts "prepare", "start", "sleep", "awake", "suspend", "resume" events when the condition is met.
```cpp
#include "ardukit.h"

GTask t1, t2;

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
    t1.bind(taskFunc, 1000).start();    // bind() returns GTask itself
    t2.bind(taskFunc, 2000).start();
}

void loop() {
    adk::run();
}
```

#### Timer events
```cpp
#include "ardukit.h"

int brightness = 0;
int delta = 5;

void setLED(GEvent &e) {
    dmsg("LED brightness: %d", brightness);
    brightness += delta;
    if (brightness == 255 || brightness == 0) delta = -delta;
    GTimer::setTimeout(setLED, 10);
}

void setup() {
    Serial.begin(128000);
    GTimer::setTimeout(setLED);
}

void loop() {
    adk::run();
}
```

#### Event Emitter
```cpp
GButton b1(8, INPUT_PULLUP);

void onButtonEvent(GEvent &e)
{
    dmsg("Button Event: %s", e.eventName());
};

void setup()
{
    Serial.begin(128000);
    b1.enable();    // activate change detection
    b1.on("press", onButtonEvent);
    b1.on("releas", onButtonEvent);
}

void loop()
{
    adk::run();
}
```

GButton uses GEventEmitter to trigger "press" or "release" event when change detected.


## Library Source
Check [here](lib/ardukit)


## Examples
To test examples, copy one from lib/ardukit/examples directory to src/.
