/**
 *  @package Ardukit
 *
 *  @module util
 */

#pragma once

#ifndef ADK_SERIAL_BUFFER_SIZE
#define ADK_SERIAL_BUFFER_SIZE      128
#endif

// extern GSerialType GSerial;
void dmsg(const char *format, ...);
