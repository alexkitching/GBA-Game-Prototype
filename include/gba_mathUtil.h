
#ifndef __GBA_MATHUTIL_H__
#define __GBA_MATHUTIL_H__

#include "gba_types.h"

extern s32 __qran_seed;
extern s32 sqran(s32 a_val);
extern s32 qran();
extern int qran_range(s32 a_min, s32 a_max);
extern s32 sign(s32 a_val);
extern s32 abs(s32 a_val);

#endif //__GBA_MATHUTIL_H__
