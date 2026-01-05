#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <stdint.h>
#include <math.h>

#define VM_INLINE __forceinline
#define M_PI 3.14159265358979323846f
#define DEG2RAD(_a) ((_a)*M_PI/180.0f)
#define RAD2DEG(_a) ((_a)*180.0f/M_PI)
#define INT_MIN (-2147483647 - 1)
#define INT_MAX 2147483647
#define FLT_MAX 3.402823466e+38F