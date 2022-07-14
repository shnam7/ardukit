/**
 *  @package Ardukit
 *
 *  @module util
 */

#include "gutil.h"
#include <stdio.h>

char *adk::u64_to_str(char *outbuf, unsigned long long n, unsigned base)
{
    unsigned char buf[16 * sizeof(long)]; // Assumes 8-bit chars.
    if (n == 0) {
        outbuf[0] = '0';
        outbuf[1] = 0;
        return outbuf;
    }

    int i = 0;
    while (n > 0) {
        buf[i++] = n % base;
        n /= base;
    }

    int k = 0;
    // dmsg("i=%d\n", i);
    for (; --i >= 0;) {
        // dmsg("i=%d\n", i);
        outbuf[k++] = (char)(buf[i] < 10 ? '0' + buf[i] : 'A' + buf[i] - 10);
    }
    outbuf[k] = 0; // end of striing
    return outbuf;
}
