# ardukit
ardukit is a foundation library toolkit for easier arduino programming.


## Modules included:
- task: Periodic task manaer
- event: EventEmitter
- list: simple doubly linked list
- que: simple circular Queue
- time: time ticks and conversion
- timer: timer callback service
- util: debuging functions, etc.
- button: Noise filteriing button input handler
- edge_detector: Noise filtering, riging and falling edge detector for analog input devices
- mv_filter: Moving Average filter


## Notes
ardukit is using namespace **adk** for all public identifiers except dmsg().
So, to use ardukit classes or functions, namespace declaration is required.

    ```cpp
    using namespace adk;
    ```

Or, **adk** should be prefixed to classes or functions like this:

    ```cpp
    adk::Button b1(8, INPUT_PULLUP);        // Button class instance
    adk::dmsg(...);                         // function call
    ```

And, be sure to call **adk::run()** in the loop() function to enable ardukit engine, which is running Task instances, timer callback functions, etc.

This is a typical source code format to use ardukit.
```cpp
#include <adrukit.h>

using namespace adk;

void setup()
{
    // ...
}

void loop()
{
    adk::run(); // run ardukit engine
}

```


## Quick Examples

### Non-blocking periodic function calls using timer service

This example toggles the LED every 5 seconds, concurrently checking input button every 200 milliseconds with no blocking.

```cpp
#include <ardukit.h>

void toggle_led(void *) { /* ... */ }
void check_input_button(void *) { /* ... */ }

void setup()
{
    // call toggle_led() function in every 3 seconds
    adk::set_interval(toggle_led, 3000);

    // check input for every 200 milliseconds
    adk::set_interval(check_input_button, 200);
}

void loop()
{
    adk::run(); // run ardukit engine
}
```

### Another example using timer service

```cpp
#include <ardukit.h>
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

    // reserve calling set_LED() after 2 sec in the loop()
    set_timeout(set_LED, edge ? 2000 : 0);
}

void setup()
{
    Serial.begin(115200);

    // reserve calling set_LED() after 0 sec in the loop()
    set_timeout(set_LED, 0);
}

void loop()
{
    adk::run(); // run ardukit engine
}
```

### Using event driven button
Button triggers "press" and "release" events when changes are detected.

```cpp
#include <ardukit.h>
using namespace adk;

adk::Button b1(8, INPUT_PULLUP);

void on_button_event(adk::EventEmitter::event &e)
{
    dmsg("Button Event: %s", e.name);
};

void setup()
{
    Serial.begin(115200);
    b1.enable();    // activate change detection
    b1.on("press", on_button_event);
    b1.on("release", on_button_event);
}

void loop()
{
    adk::run(); // run ardukit engine
}
```

### Easy non-preemptive multi-tasking.

This is a simple example running multiple periodic tasks.
You can refer to the examples folder for more advanced use cases.

```cpp
#include <ardukit.h>
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
    t2.set_interval(2000).start(task_func); // periodic task w/ interval=2sec
    t3.set_interval(3000).start(task_func); // periodic task w/ interval=3sec
}

void loop()
{
    adk::run(); // run ardukit engine
}
```
