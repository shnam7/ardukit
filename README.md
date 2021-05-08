# Ardukit
Ardukit is a foundation library toolkit for easier arduino programming.

## Modules included:
- gtask: Periodic task manaer
- gevent: EventEmitter
- glist: simple doubly linked list
- gque: simple circular Queue
- gtime: time ticks and conversion
- gtimer: timer callback services
- gutil: debuging functions, etc.
- gbutton: Noise filteriing button input handler
- gedge_trigger: Noise filtering analog input up/down edge detector


## Quick Examples

#### Easy non-preemptive multi-tasking.
```cpp
using namespace adk;

Task t1, t2, t3;

void task_func(Task &t)
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
```

Task support EventEitter triggering "prepare", "start", "sleep", "awake", "suspend", "resume" events when the condition is met.

```cpp
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


```
#### Timer events
```cpp
#include "ardukit.h"
using namespace adk;

int brightness  = 0;
int delta       = 5;
tick_t tm0      = 0;
tick_t elapsed  = 0;

void set_LED(void *)
{
    tick_t tm1 = ticks();
    dmsg("LED brightness:%d elapsed=%d(msec)", brightness, ticks_to_msec(tm1-tm0));
    tm0 = tm1;

    brightness += delta;
    bool edge = brightness == 255 || brightness == 0;
    if (edge) delta = -delta;
    set_timeout(set_LED, edge ? 2000 : 0);
}

void setup()
{
    Serial.begin(128000);
    set_timeout(set_LED, 0);
}

void loop()
{
    adk::run();
}
```

#### Event Emitter
```cpp
void on_button_event(adk::EventEmitter::event &e)
{
    dmsg("Button Event: %s", e.name);
};

adk::Button b1(8, INPUT_PULLUP);

void setup()
{
    Serial.begin(128000);
    b1.enable();    // activate change detection
    b1.on("press", on_button_event);
    b1.on("releas", on_button_event);
}

void loop()
{
    adk::run();
}
```

Button triggers "press" and "release" events when changes are detected.


## Note
Be sure to call adk::run() in the loop() function to enable Task and timer callback functions.


## Library Source
Check [here](lib/ardukit)


## Examples
To test examples, copy one from "lib/ardukit/examples" directory to "src/".
