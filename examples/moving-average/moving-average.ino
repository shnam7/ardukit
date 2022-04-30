/*
 *  Example: Moving Average
 */

#include "ardukit.h"
#include <stdio.h>


void setup()
{
    adk::MVFilter mvf(5);

    for (int i=1; i<=20; i++) {
        double r = mvf.filter(i * 1.3);
        int val = round(r);
        String s(r);
        dmsg("val=%d real-val=%s\n", val, s.c_str());
    }
}

void loop()
{
    // do nothing
}
