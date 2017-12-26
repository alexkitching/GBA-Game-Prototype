#ifndef __PLAYER_FLAME_H__
#define __PLAYER_FLAME_H__

/*
----------------------------------------------------------/
----- Project: GBA Assignment							  /
----- File:    player_flame.h							  /
----------------------------------------------------------/
----- Desc: Declares the PlayerFlame Struct and Fucntions./
-----		Player Flame is attached to player and based  /
-----	    on input, Starts/Stops and Updates the Flame. /
----------------------------------------------------------/
----- Author: Alex Kitching								  /
----------------------------------------------------------/
*/

#include "gba.h"
#include "object.h"
#include "object_animation_controller.h"

#define FLAME_ANIM_TIME 2
#define FLAME_START_INDEX 0
#define FLAME_LOOP_INDEX 3
#define FLAME_LOOP_END_INDEX 9

#define FLAME_PRIORITY 5

typedef struct PlayerFlame
{
	Object* poMyObject;
	ObjectAnimationController oacMyAnimController;
	bool bActive;
	// Padding
	u8 pad[3];
}PlayerFlame; // 208 Bytes

extern void SetupFlame(PlayerFlame * const a_pFlame, Object * const a_oFlameObject);
void StartFlame(PlayerFlame * const a_pFlame);
extern void UpdateFlame(PlayerFlame * const a_pFlame, const Vector a_vPosition, const bool a_bFacingDir);
extern void EndFlame(PlayerFlame * const a_pFlame);
#endif // !__PLAYERFLAME_H__
