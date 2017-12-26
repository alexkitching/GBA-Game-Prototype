#ifndef __GBA_MATH_H__
#define __GBA_MATH_H__

#include "gba_types.h"
//\======================================================================
// Fixed Point Mathematics
//\======================================================================

//This fix shift defines the number of bits reserved for decimal values
//the GBA uses 8.8 fixed values for it's affine's and transforms in OAM
#define FIX_SHIFT		8

#define FIX_SCALE		( 1 << FIX_SHIFT	)
#define FIX_MASK		( FIX_SCALE-1		)
#define FIX_SCALEF		( (float)FIX_SCALE	)
#define FIX_SCALE_INV	( 1.0f/FIX_SCALEF	)

#define FIX_ONE			FIX_SCALE

//Using a signed 32 bit value for fixed point 24.8 fixed
//The GBA uses signed 16 bit values as 8.8 fixed
//The additional 16 bits on our fixed value will allow for better numerical overflow
typedef s32 fixed;
//Functions to convert from non-fixed point representation to fixed point
extern fixed	int2fix(s32 a_i);
extern fixed	float2fix(float a_f);
//functions to convert from fixed point to non fixed point representation
extern u32		fix2uint(fixed a_f);
extern u32		fix2ufrac(fixed a_f);
extern s32		fix2int(fixed a_f);
extern float	fix2float(fixed a_f);
//Functions to undertake fixed point addition, subtraction, multiplication & division
extern fixed	fixAdd(fixed a_fa, fixed a_fb);
extern fixed	fixSub(fixed a_fa, fixed a_fb);
extern fixed	fixMul(fixed a_fa, fixed a_fb);
extern fixed	fixDiv(fixed a_fa, fixed a_fb);
//\=====================================================================================
//Look Up tables
extern s32 lu_sin(u32 theta);
extern s32 lu_cos(u32 theta);

//Vector

typedef struct Vector
{
	fixed x;
	fixed y;
} Vector;

#endif //__GBA_MATH_H__
