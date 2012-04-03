
#ifndef MATH_H_
#define MATH_H_


#include <stdlib.h>
#include <stdint.h>

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

long map_l(long x, long in_min, long in_max, long out_min, long out_max);
uint8_t map_b(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

#endif
