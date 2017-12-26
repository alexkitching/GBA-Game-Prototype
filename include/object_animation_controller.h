#ifndef __OBJECT_ANIMATION_CONTROLLER_H__
#define __OBJECT_ANIMATION_CONTROLLER_H__

/*
----------------------------------------------------------/
----- Project: GBA Assignment							  /
----- File:    object_animation_controller.h				  /
----------------------------------------------------------/
----- Desc: Declares the ObjectAnimationController Struct,/
-----       Anim Struct and Functions.                    /
-----       Animation Controller is attached to Objects	  /
-----       requiring animation. Allows for dynamic adding/
-----       of both static and dynamic animations.		  /
-----       Supports up to 5 Static Anims and 10 Dynamic  /
-----		Anims.									      /
----------------------------------------------------------/
----- Author: Alex Kitching								  /
----------------------------------------------------------/
*/

#include "gba.h"
#include "object.h"

#define MAX_STATIC_ANIM 5
#define MAX_DYNAMIC_ANIM 10

#define FRAMETIME_DECREMENT 0x080
#define ANIM_PLAYING_ID -1

typedef struct Anim
{
	const unsigned short * c_pusPallete;
	const unsigned short * c_pusTiles;

	u16 uiPalleteLength;
	u16 uiTilesLength;
} Anim; // 12 Bytes

typedef struct ObjectAnimationController
{
	Object* pObjectToAnimate; 

	fixed fixDefaultFrameTime; 
	fixed fixFrameTime;
	fixed fixIndex;

	Anim StaticAnimArray[MAX_STATIC_ANIM]; 
	Anim DynamicAnimArray[MAX_DYNAMIC_ANIM]; 

	s8 siCurrentAnimID;
	s8 siPlayRangeStartIndex;
	u8 uiStaticAnimCount;
	u8 uiDynamicAnimCount;

} ObjectAnimationController; // 200 Bytes

extern void SetupAnimController(ObjectAnimationController * const a_pAnimController, Object * const a_pObjectToAnimate, const fixed a_fixDefaultFrameTime);
extern u8 AddStaticAnim(ObjectAnimationController * const a_pAnimController, const unsigned short* a_pusAnimPal, const u16 a_uiAnimPalLength, const unsigned short* a_pusAnimTiles, const u16 a_uiAnimTilesLength);
extern void AddDynamicAnim(ObjectAnimationController * const a_pAnimController, const unsigned short* a_pusAnimPal, const u16 a_uiAnimPalLength, const unsigned short* a_pusAnimTiles, const u16 a_uiAnimTilesLength);
extern void LoadAnim(ObjectAnimationController * const a_pAnimController, const u8 a_uiAnimID);
void LoadDynamicAnim(ObjectAnimationController * const a_pAnimController, const u8 a_uiAnimID);
extern void PlayDynamicAnim(ObjectAnimationController * const a_pAnimController);


#endif // !__OBJECT_ANIMATION_CONTROLLER_H__
