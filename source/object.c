/*
----------------------------------------------------------/
----- Project: GBA Assignment							  /
----- File:    object.c			             			  /
----------------------------------------------------------/
----- Desc: Defines the Object Functions				  /
----------------------------------------------------------/
----- Author: Alex Kitching								  /
----------------------------------------------------------/
*/

#include "object.h"

// Initialises Objects Variables
void InitialiseObject(Object * const a_pObject, const s16 a_siStartPosY, const u8 a_uiObjMode, const u8 a_uiGFXMode, const u8 a_uiMosaic, const u8 a_uiColorMode, const u8 a_uiShape, const s16 a_siStartPosX, const u8 a_uiFlip, const u8 a_uiSize, const u8 a_uiPriority, const fixed a_fixStartVX, const fixed a_fixStartVY, const bool a_bStartFacingDir)
{
	a_pObject->poaAttrib->attr0 = SetAttrib0(a_siStartPosY, a_uiObjMode, a_uiGFXMode, a_uiMosaic, a_uiColorMode, a_uiShape);
	a_pObject->poaAttrib->attr1 = SetAttrib1(a_siStartPosX, a_uiFlip, a_uiSize);
	a_pObject->poaAttrib->attr2 = SetAttrib2(a_pObject->uiObjTileStartAddress, a_uiPriority, a_pObject->uiObjID);
	a_pObject->vPosition.x = int2fix(a_siStartPosX);
	a_pObject->vPosition.y = int2fix(a_siStartPosY);
	a_pObject->vVelocity.x = a_fixStartVX;
	a_pObject->vVelocity.y = a_fixStartVY;
	a_pObject->bFacingDirection = a_bStartFacingDir;
}

// Sets an Object's Padding
void SetObjectPadding(Object * const a_pObject, const u8 a_uiPadding)
{
	a_pObject->poaAttrib->pad = a_uiPadding;
}

// Flips an Object Horizontally
void FlipObjectHorizontal(Object * const a_pObject)
{
	a_pObject->poaAttrib->attr1 ^= A1_HFLIP;
}

// Flips an Object Vertically
void FlipObjectVertical(Object * const a_pObject)
{
	a_pObject->poaAttrib->attr1 ^= A1_VFLIP;
}

// Moves an Object to a specified Position
void MoveObject(Object * const a_pObject, const fixed a_fixPosX, const fixed a_fixPosY)
{
	a_pObject->vPosition.x = a_fixPosX;
	a_pObject->vPosition.y = a_fixPosY;
	SetObjectPosition(a_pObject->poaAttrib, fix2int(a_pObject->vPosition.x), fix2int(a_pObject->vPosition.y));
}

// Updates an Object using its Velocity and Position
void UpdateObject(Object * const a_pObject)
{
	a_pObject->vPosition.x = fixAdd(a_pObject->vPosition.x, fixMul(fixMul(a_pObject->vVelocity.x, FRAMETIME), PIXELS2METERS));
	a_pObject->vPosition.y = fixAdd(a_pObject->vPosition.y, fixMul(fixMul(a_pObject->vVelocity.y, FRAMETIME), PIXELS2METERS));
	SetObjectPosition(a_pObject->poaAttrib, fix2int(a_pObject->vPosition.x), fix2int(a_pObject->vPosition.y));
}
