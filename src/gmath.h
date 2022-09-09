/**
 *  @package Ardukit
 *
 *  @module math
 */

#pragma once
#include <math.h>
#include <float.h>

#ifndef REAL_EPSILON
#define REAL_EPSILON	DBL_EPSILON
#endif

#ifndef REAL_MIN
#define REAL_MIN	    DBL_MIN
#endif

#ifndef REAL_MAX
#define REAL_MAX	    DBL_MAX
#endif


//--------------------------------------------------------------------
// Math definitions/algorithms
//--------------------------------------------------------------------
namespace adk {

namespace math {

typedef double real;

inline bool is_zero(real x) { return (x >= -REAL_EPSILON && x <= REAL_EPSILON); }
inline long round_up(real x) { return (long)ceil(x); }
inline long round_dn(real x) { return (long)floor(x); }
inline real degree(real rad) { return rad * 180.0 / M_PI; }
inline real radian(real deg) { return deg * M_PI / 180.0; }
inline real phase(real x, real y) { real a = atan2(y, x); return (a < 0) ? a + M_2_PI : a; }

} // namespace math

} // namespace adk
