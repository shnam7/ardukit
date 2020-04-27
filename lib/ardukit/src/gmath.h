/**
 *  @package Ardukit
 *
 *  @module math
 */

#pragma once
#include <math.h>
#include <float.h>
#include "adkdef.h"

#ifndef REAL_EPSILON
#define REAL_EPSILON	DBL_EPSILON
#endif

#ifndef REAL_MIN
#define REAL_MIN	    DBL_MIN
#endif

#ifndef REAL_MAX
#define REAL_MAX	    DBL_MAX
#endif

typedef double			real;

//--------------------------------------------------------------------
// Math definitions/algorithms
//--------------------------------------------------------------------
namespace GMath {
    inline bool isZero(real x) { return (x >= -REAL_EPSILON && x <= REAL_EPSILON); }
    inline long roundUp(real x) { return (long)ceil(x); }
    inline long roundDn(real x) { return (long)floor(x); }
    inline real degree(real rad) { return rad * 180.0 / M_PI; }
    inline real radian(real deg) { return deg * M_PI / 180.0; }
    inline real phase(real x, real y) { real a = atan2(y, x); if (a < 0) a += M_2_PI; return a; }
}
