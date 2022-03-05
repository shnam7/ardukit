#include <ardukit.h>
#include "pitches.h"

// notes in the melody:
const int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
const int noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4
};

int thisNote = 0;
unsigned int timeoutID = 0;
adk::Button b1(2, INPUT_PULLUP);

void play_note(void *)
{
    // stop the tone playing:
    noTone(8);
    
    // to calculate the note duration, take one second divided by the note type.
    // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    if (++thisNote < 8)
        timeoutID = adk::set_timeout(play_note, pauseBetweenNotes);
}

void on_button_press(adk::EventEmitter::event &e) {
    thisNote = 0;
    adk::clear_timeout(timeoutID);
    timeoutID = adk::set_timeout(play_note, 0);
}

void setup()
{
    b1.enable();
    b1.on("press", on_button_press);
    // adk::set_timeout(play_note, 0);    
}

void loop()
{
    adk::run();
}