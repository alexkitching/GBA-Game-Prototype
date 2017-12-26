
#ifndef __GBA_DRAWING_H__
#define __GBA_DRAWING_H__

#include "gba_reg.h"
#include "gba_types.h"
#include "gba_mathUtil.h"

#define SCREENBUFFER ((vu16*)(VRAM))

#define SCREEN_W 240
#define SCREEN_H 160

extern u16 setColor(u8 a_red, u8 a_green, u8 a_blue);
extern void drawRect(u32 a_left, u32 a_top, u32 a_width, u32 a_height, u16 a_color);
extern void drawLine(s32 a_x, s32 a_y, s32 a_x2, s32 a_y2, u16 a_color);

#endif //__GBA_DRAWING_H__
