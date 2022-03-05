//
//  Non-blocking Blink example not using delay()
//

#include <ardukit.h>

int ledValue = HIGH;

void blink(void *) {
  digitalWrite(LED_BUILTIN, ledValue);
  ledValue = ledValue == HIGH ? LOW : HIGH;
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  adk::set_interval(blink, 1000);
}

// the loop function runs over and over again forever
void loop() {
  adk::run();
}
