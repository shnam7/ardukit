/**
 *  @package Ardukit
 *
 *  @module adkdef - basic definitions
 */

#pragma once
#include <stdint.h>

//--- types
typedef int8_t      i8_t;
typedef int16_t     i16_t;
typedef int32_t     i32_t;
typedef int64_t     i64_t;

typedef uint8_t     u8_t;
typedef uint16_t    u16_t;
typedef uint32_t    u32_t;
typedef uint64_t    u64_t;
typedef uintptr_t   uptr_t;


//--- debug utils
void dmsg(const char *fmt, ...);
