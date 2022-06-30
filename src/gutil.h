/**
 *  @package Ardukit
 *
 *  @module util
 */

#pragma once

#ifndef ADK_SERIAL_BUFFER_SIZE
#define ADK_SERIAL_BUFFER_SIZE      128
#endif

// printf() equivalent pring messages to serial port (dmsg name comes from 'debug message')
// this function is declared in global name space, not adk namespace, for convenience of use
void dmsg(const char *format, ...);


namespace adk
{

// convert 64-bit number into string
char *u64_to_str(char *buf, unsigned long long n, unsigned base=10);

} // namespace adk
