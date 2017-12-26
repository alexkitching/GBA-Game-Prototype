#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

/*
----------------------------------------------------------/
----- Project: GBA Assignment							  /
----- File:    game_manager.h							  /
----------------------------------------------------------/
----- Desc: Declares the Game Manager Struct and          /
-----       Functions. The Game Manager is Mainly         /
-----		Responsible for Initialising the Game as well /
-----		as Running the Main Loop.                     /
----------------------------------------------------------/
----- Author: Alex Kitching								  /
----------------------------------------------------------/
*/

#include "gba.h"
#include "player.h"
#include "loader.h"
#include "background_manager.h"

typedef struct GameManager
{
	Loader loader;
	Player player;
	BackgroundManager bgManager;
}GameManager; // 6708 Bytes 

extern void InitGame(GameManager * const a_pManager);
extern void UpdateGame(GameManager * const a_pManager);
void HandlePlayerYBoundJump(Player * const a_pPlayer, const Vector a_vInputVelocity);
bool PlayerHasControl(const fixed a_fixMapPosX, const fixed a_fixPlayerX, const Vector a_vPlayerInput);

#endif // !__GAME_MANAGER_H__
