/*
----------------------------------------------------------/
----- Project: GBA Assignment							  /
----- File:    player_flame.c							  /
----------------------------------------------------------/
----- Desc: Defines the Player Flame Functions			  /
----------------------------------------------------------/
----- Author: Alex Kitching								  /
----------------------------------------------------------/
*/

#include "player_flame.h"
#include "flame.h"

// Initialises Flame Object as well as Assigning Object to Player Flame. Adds the numerous Animations to the Animation Controller.
extern void SetupFlame(PlayerFlame * const a_pFlame, Object * const a_oFlameObject)
{
	// Assign Object
	a_pFlame->poMyObject = a_oFlameObject;
	// Initialise Object
	InitialiseObject(a_oFlameObject, 0, A0_MODE_HIDE, 0, 0, A0_4BPP, A0_WIDE, 0, 0, A1_SIZE_2, a_oFlameObject->uiObjTileStartAddress, FLAME_PRIORITY, a_oFlameObject->uiObjID, LEFTFACE);

	// Flame Default to Inactive
	a_pFlame->bActive = false;

	// Animation Setup
	SetupAnimController(&a_pFlame->oacMyAnimController, a_oFlameObject, float2fix(FLAME_ANIM_TIME));
	// Add Start Anims
	AddDynamicAnim(&a_pFlame->oacMyAnimController, flamestart1Pal, flamestart1PalLen, flamestart1Tiles, flamestart1TilesLen);
	AddDynamicAnim(&a_pFlame->oacMyAnimController, flamestart2Pal, flamestart2PalLen, flamestart2Tiles, flamestart2TilesLen);
	AddDynamicAnim(&a_pFlame->oacMyAnimController, flamestart3Pal, flamestart3PalLen, flamestart3Tiles, flamestart3TilesLen);
	// Add Loop Anims
	AddDynamicAnim(&a_pFlame->oacMyAnimController, flameloop1Pal, flameloop1PalLen, flameloop1Tiles, flameloop1TilesLen);
	AddDynamicAnim(&a_pFlame->oacMyAnimController, flameloop2Pal, flameloop2PalLen, flameloop2Tiles, flameloop2TilesLen);
	AddDynamicAnim(&a_pFlame->oacMyAnimController, flameloop3Pal, flameloop3PalLen, flameloop3Tiles, flameloop3TilesLen);
	AddDynamicAnim(&a_pFlame->oacMyAnimController, flameloop4Pal, flameloop4PalLen, flameloop4Tiles, flameloop4TilesLen);
	AddDynamicAnim(&a_pFlame->oacMyAnimController, flameloop5Pal, flameloop5PalLen, flameloop5Tiles, flameloop5TilesLen);
	AddDynamicAnim(&a_pFlame->oacMyAnimController, flameloop6Pal, flameloop6PalLen, flameloop6Tiles, flameloop6TilesLen);
	AddDynamicAnim(&a_pFlame->oacMyAnimController, flameloop7Pal, flameloop7PalLen, flameloop7Tiles, flameloop7TilesLen);
}

// Activates the Flame
void StartFlame(PlayerFlame * const a_pFlame)
{
	// Unhides Flame
	obj_unhide(a_pFlame->poMyObject->poaAttrib, A0_MODE_REG);
	// Reset Anim Index
	a_pFlame->oacMyAnimController.fixIndex = FLAME_START_INDEX;
	// Now Active
	a_pFlame->bActive = true;
}

// Updates the Flame
void UpdateFlame(PlayerFlame * const a_pFlame, const Vector a_vPosition, const bool a_bFacingDir)
{
	if (a_pFlame->poMyObject->bFacingDirection != a_bFacingDir) // Flip Flame if needed
	{
		a_pFlame->poMyObject->bFacingDirection = a_bFacingDir;
		FlipObjectHorizontal(a_pFlame->poMyObject);
	}

	if (!a_pFlame->bActive) // Flame not active
	{
		StartFlame(a_pFlame); // Flame now active
	}

	// Loop through animation
	PlayDynamicAnim(&a_pFlame->oacMyAnimController);

	if (a_pFlame->oacMyAnimController.fixIndex == FLAME_LOOP_END_INDEX) // Ensure only loops the LOOP part of the animation - Not repeat Start Flame Anim
	{
		a_pFlame->oacMyAnimController.fixIndex = FLAME_LOOP_INDEX;
	}

	// Update Flame Position
	SetObjectPosition(a_pFlame->poMyObject->poaAttrib, fix2int(a_vPosition.x), fix2int(a_vPosition.y));
}

// Stops the Flame
extern void EndFlame(PlayerFlame * const a_pFlame)
{
	// Hide Flame
	obj_hide(a_pFlame->poMyObject->poaAttrib);
	// No Longer Active
	a_pFlame->bActive = false;
}
