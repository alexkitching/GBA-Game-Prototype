/*
----------------------------------------------------------/
----- Project: GBA Assignment							  /
----- File:    object_animation_controller.h			  /
----------------------------------------------------------/
----- Desc: Defines the ObjectAnimationController         /
-----       Functions.									  /
----------------------------------------------------------/
----- Author: Alex Kitching								  /
----------------------------------------------------------/
*/

#include "object_animation_controller.h"
#include "loader.h"

// Initialises Anim Controller Variables
void SetupAnimController(ObjectAnimationController * const a_pAnimController, Object * const a_pObjectToAnimate, const fixed a_fixDefaultFrameTime)
{
	a_pAnimController->pObjectToAnimate = a_pObjectToAnimate;
	a_pAnimController->fixDefaultFrameTime = a_fixDefaultFrameTime;
	a_pAnimController->fixFrameTime = a_fixDefaultFrameTime;
	a_pAnimController->fixIndex = 0;
	a_pAnimController->siCurrentAnimID = NULL;
	a_pAnimController->uiStaticAnimCount = 0;
	a_pAnimController->uiDynamicAnimCount = 0;
}

// Adds a Static Anim and Returns corresponding ID to Object
extern u8 AddStaticAnim(ObjectAnimationController * const a_pAnimController, const unsigned short* a_pusAnimPal, const u16 a_uiAnimPalLength, const unsigned short* a_pusAnimTiles, const u16 a_uiAnimTilesLength)
{
	// Create our new Anim
	Anim newAnim;
	newAnim.c_pusPallete = a_pusAnimPal;
	newAnim.uiPalleteLength = a_uiAnimPalLength;
	newAnim.c_pusTiles = a_pusAnimTiles;
	newAnim.uiTilesLength = a_uiAnimTilesLength;

	if (a_pAnimController->uiStaticAnimCount < MAX_STATIC_ANIM) // We can Assign Static Anim
	{
		a_pAnimController->StaticAnimArray[a_pAnimController->uiStaticAnimCount] = newAnim;
		// Increase Static Anim Count
		++a_pAnimController->uiStaticAnimCount;
		// Return our Anim ID
		return a_pAnimController->uiStaticAnimCount - 1;
	}
	else
	{
		return -1; // Unable to Assign
	}
}

// Adds a Dynamic Anim to the Controller
extern void AddDynamicAnim(ObjectAnimationController * const a_pAnimController, const unsigned short* a_pusAnimPal, const u16 a_uiAnimPalLength, const unsigned short* a_pusAnimTiles, const u16 a_uiAnimTilesLength)
{
	// Create our new Anim
	Anim newAnim;
	newAnim.c_pusPallete = a_pusAnimPal;
	newAnim.uiPalleteLength = a_uiAnimPalLength;
	newAnim.c_pusTiles = a_pusAnimTiles;
	newAnim.uiTilesLength = a_uiAnimTilesLength;

	if (a_pAnimController->uiDynamicAnimCount < MAX_DYNAMIC_ANIM) // We can Assign Dynamic Anim
	{
		a_pAnimController->DynamicAnimArray[a_pAnimController->uiDynamicAnimCount] = newAnim;
		// Increase Dynamic Anim Count
		++a_pAnimController->uiDynamicAnimCount;
	}
}

// Loads a Specified Static Anim via ID
extern void LoadAnim(ObjectAnimationController * const a_pAnimController, const u8 a_uiAnimID)
{
	if (a_uiAnimID + 1 <= a_pAnimController->uiStaticAnimCount && a_uiAnimID >=  0) // Anim is Valid
	{
		LoadToExistingObject(a_pAnimController->pObjectToAnimate, a_pAnimController->StaticAnimArray[a_uiAnimID].c_pusPallete, a_pAnimController->StaticAnimArray[a_uiAnimID].uiPalleteLength, a_pAnimController->StaticAnimArray[a_uiAnimID].c_pusTiles, a_pAnimController->StaticAnimArray[a_uiAnimID].uiTilesLength);
		a_pAnimController->siCurrentAnimID = a_uiAnimID;
	}
}

// Loads a Specified Dynamic Anim via ID
void LoadDynamicAnim(ObjectAnimationController * const a_pAnimController, const u8 a_uiAnimID)
{
	if (a_uiAnimID + 1 <= a_pAnimController->uiDynamicAnimCount && a_uiAnimID >= 0) // Anim is Valid
	{
		LoadToExistingObject(a_pAnimController->pObjectToAnimate, a_pAnimController->DynamicAnimArray[a_uiAnimID].c_pusPallete, a_pAnimController->DynamicAnimArray[a_uiAnimID].uiPalleteLength, a_pAnimController->DynamicAnimArray[a_uiAnimID].c_pusTiles, a_pAnimController->DynamicAnimArray[a_uiAnimID].uiTilesLength);
	}
}

// Loops and Plays Dynamic Anims
extern void PlayDynamicAnim(ObjectAnimationController * const a_pAnimController)
{
	if (a_pAnimController->siCurrentAnimID != ANIM_PLAYING_ID) // CurrentAnimID = -1 During Playing of Animation
	{
		a_pAnimController->siCurrentAnimID = ANIM_PLAYING_ID;
	}
	// Decrease FrameTime
	a_pAnimController->fixFrameTime -= FRAMETIME_DECREMENT;

	if (a_pAnimController->fixFrameTime < 0) // Frame time elapsed
	{
		if (a_pAnimController->fixIndex != a_pAnimController->uiDynamicAnimCount) // Anim Index not reached Max Index
		{
			++a_pAnimController->fixIndex;
		}
		else // Reset Anim Index
		{
			a_pAnimController->fixIndex = 0;
		}

		// Reset Frame TIme
		a_pAnimController->fixFrameTime = a_pAnimController->fixDefaultFrameTime;

		// Load Anim
		LoadDynamicAnim(a_pAnimController, a_pAnimController->fixIndex);
	}
}
