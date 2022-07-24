#ifndef FIXED_ARITHMETIC_H
#define FIXED_ARITHMETIC_H


#include <stdint.h>

/*
   This header file defines macros for working with fixed-point real numbers
   that are represented by 32-bit integers, using the low-order 14 bits of
   an integer to represent the fractional part of the real number.  The
   formulas are from Section B.6 of the manual for the Pintos operating
   system.
*/

typedef int32_t fixed_t; // Define the type name fixed_t to be an alias of int32_t,
                         // meant for declaring variables that will be used to
                         // represent FIXED real values.

#define FIXED_FRACTION  (2 << 14)
          // 14 bits used for fractional part (17.14 fixed represenation)
#define FIXED_HALF_FRACTION  (2 << 13)

/* Convert an integer to its FIXED representation.  (abs(n) must be < 2^17) */
#define INT2FIXED(n) ((n) * FIXED_FRACTION)

/* FIXED2INT rounds to nearest int; FIXED2INT_TRUNC truncates fractional part */
#define FIXED2INT(x) ( ((x) < 0)? ((x)-FIXED_HALF_FRACTION)/FIXED_FRACTION : ((x)+FIXED_HALF_FRACTION)/FIXED_FRACTION )
#define FIXED2INT_TRUNC(x) ((x)/FIXED_FRACTION)

/* Arithmetic operations applied to two FIXED values. */
#define FIXED_ADD(x,y) ((x)+(y))
#define FIXED_SUB(x,y) ((x)-(y))
#define FIXED_MUL(x,y) ((((int64_t)(x))*(y)) / FIXED_FRACTION)
#define FIXED_DIV(x,y) ((((int64_t)(x))*FIXED_FRACTION) / (y))

/* Arithmetic operations applied to a FIXED value and a reqular integer. */
#define FIXED_ADD_INT(x,n) ((x) + n*FIXED_FRACTION)
#define FIXED_SUB_INT(x,n) ((x) - n*FIXED_FRACTION)
#define FIXED_MUL_INT(x,n) ((x)*(n))
#define FIXED_DIV_INT(x,n) ((x)/(n))

/* The next two can't be used in Pintos, which does not support floating-point numbers in the kernel. */
/*
#define FIXED2FLOAT(x) ((double)(x) / (double)FIXED_FRACTION)
#define FLOAT2FIXED(x) ( ((x) < 0)? (int64_t)((x)*FIXED_FRACTION - 0.5) : (int64_t)((x)*FIXED_FRACTION + 0.5))
*/

#endif
