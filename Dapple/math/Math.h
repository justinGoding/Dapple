#pragma once

#include "..\core\Core.h"

#include "VectorMath.h"
#include "MatrixMath.h"

#define DEG * 0.017453292519943f

// Random number generator 
global_var unsigned int seed = 0x13371337;

inline float random_float()
{
	float res;
	unsigned int tmp;

	seed *= 16807;

	tmp = seed ^ (seed >> 4) ^ (seed << 15);

	*((unsigned int*)&res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}