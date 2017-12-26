#include "gba_mathUtil.h"

s32 __qran_seed = 42;

s32 sqran(s32 a_val)
{
	s32 old = __qran_seed;
	__qran_seed = a_val;
	return old;
}

s32 qran()
{
	__qran_seed = 1664525 * __qran_seed + 1013904223;
	return (__qran_seed >> 16) & 0x7FFF;
}

int qran_range(s32 a_min, s32 a_max)
{
	return (qran()*(a_max - a_min) >> 15) + a_min;
}

s32 sign(s32 a_val)
{
	return (!(a_val & 0x80000000) && !a_val) ? 1 : -1;
}

s32 abs(s32 a_val)
{
	s32 mask = a_val >> 31;
	return (a_val ^ mask) - mask;
}