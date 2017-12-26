#ifndef __OBJECT_H__
#define __OBJECT_H__

/*
----------------------------------------------------------/
----- Project: GBA Assignment							  /
----- File:    object.h						  /
----------------------------------------------------------/
----- Desc: Declares the object Struct, and Functions	  /
-----       Objects hold Positions, Velocities as well as /
-----       The Object ID, Tile Addresses etc for easy way/
-----       handle changing of animation or palette.	  /
----------------------------------------------------------/
----- Author: Alex Kitching								  /
----------------------------------------------------------/
*/

#include "gba.h"

// Frame Time
#define FRAMETIME 0x04
// Pixels to Meters Conversion 
#define PIXELS2METERS 32 << FIX_SHIFT

// Left/Right Facing Direction
#define LEFTFACE false
#define RIGHTFACE true

typedef struct Object
{
	bool bFacingDirection;
	u8 uiObjID;
	u16 uiObjTileStartAddress;
	ObjectAttribute* poaAttrib;
	Vector vPosition;
	Vector vVelocity; 

	// Padding - Ensure Power of 2
	u8 pad[8];
} Object;
// 32 Bytes Aligned

extern void InitialiseObject(Object * const a_pObject, const s16 a_siStartPosY, const u8 a_uiObjMode, const u8 a_uiGFXMode, const u8 a_uiMosaic, const u8 a_uiColorMode, const u8 a_uiShape, const s16 a_siStartPosX, const u8 a_uiFlip, const u8 a_uiSize, const u8 a_uiPriority, const fixed a_fixStartVX, const fixed a_fixStartVY, const bool a_bStartFacingDir);
extern void SetObjectPadding(Object * const a_pObject, const u8 a_uiPadding);
extern void FlipObjectHorizontal(Object * const a_pObject);
extern void FlipObjectVertical(Object * const a_pObject);
extern void MoveObject(Object * const a_pObject, const fixed a_fixPosX, const fixed a_fixPosY);
extern void UpdateObject(Object * const a_pObject);
#endif 