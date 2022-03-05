#include <ardukit.h>

int ledPin = 9;    // LED connected to digital pin 9
int fadeValue = 0;
int fadeDelta = 5;
int fadeInterval = 30;

void update_led(void *) {
    analogWrite(ledPin, fadeValue);
    fadeValue += fadeDelta;
    if (fadeValue<=0 || fadeValue>=255) fadeDelta *= -1;
}

void setup() {
    adk::set_interval(update_led, fadeInterval);
}

void loop() {
  adk::run();
}