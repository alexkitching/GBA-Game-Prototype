#include "gba_math.h"

//convert and integer to a fixed point representation
fixed	int2fix(s32 a_i)
{
	return a_i << FIX_SHIFT;
}
//Convert a float to a fixed point representation
fixed	float2fix(float a_f)
{
	return (fixed)(a_f*FIX_SCALEF);
}

//convert a fixed point value into an integer value
u32		fix2uint(fixed a_f)
{
	return a_f >> FIX_SHIFT;
}
//Get only the fractional component of a fixed point value as an integer
u32		fix2ufrac(fixed a_f)
{
	return a_f & FIX_MASK;
}
//convert a fixed point number into a signed int
s32		fix2int(fixed a_f)
{
	return a_f / FIX_SCALE;
}

float	fix2float(fixed a_f)
{
	return a_f / FIX_SCALEF;
}

//For these functions to perform correctly fixed point numbers must be in 
//the same format. The format for these functions is 24.8
//Adding two fixed point values together
fixed	fixAdd(fixed a_fa, fixed a_fb)
{
	return a_fa + a_fb;
}
//subtract two fixed point values
fixed	fixSub(fixed a_fa, fixed a_fb)
{
	return a_fa - a_fb;
}
//multiply two fixed point values together
//when multiplying the number of fractional bits needs to be adjusted
//a_fa has FIX_SHIFT decimal bits & a_fb has FIX_SHIFT decimal bits
//this effectively shifts the number up an additional FIX_SHIFT number of bits
//the result must be shifted down by FIX_SHIFT bits
fixed	fixMul(fixed a_fa, fixed a_fb)
{
	return (a_fa * a_fb) >> FIX_SHIFT;
}

fixed	fixDiv(fixed a_fa, fixed a_fb)
{
	return ((a_fa)* FIX_SCALE) / a_fb;
}

#include "sinlut.h"

s32 lu_sin(u32 theta)
{
	return sin_lut[(theta >> 7) & 0x1FF];
}

s32 lu_cos(u32 theta)
{
	return sin_lut[((theta >> 7) + 128) & 0x1FF];
}