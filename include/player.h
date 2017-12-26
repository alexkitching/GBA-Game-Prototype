#ifndef __PLAYER_H__
#define __PLAYER_H__

/*
----------------------------------------------------------/
----- Project: GBA Assignment							  /
----- File:    player.h						              /
----------------------------------------------------------/
----- Desc: Declares the Player Struct, Player Checkpoint /
-----       Snapshot Struct and Functions.				  /
-----		Player is Controlled Via Input and outputs a  /
-----       Velocity Vector which is then checked using   /
-----       collision detection and altered if needed.	  /
-----       Player is updated separately to handling input./
----------------------------------------------------------/
----- Author: Alex Kitching								  /
----------------------------------------------------------/
*/

#include "gba.h"
#include "object.h"
#include "object_animation_controller.h"
#include "background_manager.h"
#include "player_flame.h"

#define PLAYER_MOVING_ANIM_TIME 1
#define PLAYER_DIMENSION 32

#define FLAME_YOFFS 12

// Move Directions
#define MOVE_LEFT true
#define MOVE_RIGHT false

#define PLAYER_ACCELERATION 0x080
#define PLAYER_JUMP_FORCE -4

#define PLAYER_START_LIVES 3

typedef enum
{
	STANDING,
	MOVING,
	JUMPING,
	ATTACKING
}PlayerState;

typedef struct PlayerCheckpointSnapShot
{
	Vector vPlayerPos;
	Vector vMapPos;
	PlayerState psCurrentState;
	bool bGrounded;
	bool bCheckPointReached;

	// Padding
	u8 pad[10];
} PlayerCheckpointSnapShot; // 32 Bytes

typedef struct Player
{
	Object* poMyObject; 
	ObjectAnimationController oacMyAnimController;  

	PlayerState psCurrentState;
	bool bMoveDir;
	bool bGrounded;
	bool bCheckPointReached;
	u8 uiLives;
	PlayerCheckpointSnapShot pcsMySnap;
	MapCheckpointSnapShot pcsMapSnap;
	Vector vMapPos;

	u8 uiStaticAnimID;
	u8 uiJumpAnimID;
	u8 uiAttackAnimID;
	// Padding
	u8 pad;

	PlayerFlame pfMyFlame;

} Player; // 616 Bytes

extern void SetupPlayer(Player * const a_pPlayer, Object * const a_pPlayerObject, Object * const a_poFlameObject, BackgroundManager * const a_pBackgroundManager);
extern Vector GetPlayerInput(Player * const a_pPlayer, BackgroundManager * const a_pBackgroundManager);
void Move(Player * const a_pPlayer);
void ApplyHorizontalDrag(Player * const a_pPlayer);
void Jump(Player * const a_pPlayer);
void ResetPlayer(Player * const a_pPlayer);
Vector CheckCollisions(Player * const a_pPlayer, BackgroundManager * const a_pBackgroundManager, Vector a_pPlayerInput);
void SaveCheckPointSnapShot(Player * const a_pPlayer, BackgroundManager * const a_pBackgroundManager);
extern void UpdatePlayer(Player * const a_pPlayer, const Vector a_vVelocity);
#endif // !__PLAYER_H__
