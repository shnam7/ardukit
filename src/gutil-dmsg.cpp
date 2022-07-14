/**
 *  @package Ardukit
 *
 *  @module util
 */

#include "gutil.h"
#include "platform.h"
#include <stdio.h>

// debug message to serial port
void dmsg(const char *format, ...) {
    static char buf[ADK_SERIAL_BUFFER_SIZE];
    va_list arg_ptr;
    va_start(arg_ptr, format);

    vsnprintf(buf, ADK_SERIAL_BUFFER_SIZE - 1, format, arg_ptr);
    buf[ADK_SERIAL_BUFFER_SIZE-1] = 0;

    Serial.print(buf);
    va_end(arg_ptr);
}
