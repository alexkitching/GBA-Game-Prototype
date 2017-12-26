/*
----------------------------------------------------------/
----- Project: GBA Assignment							  /
----- File:    gamemanager.c							  /
----------------------------------------------------------/
----- Desc: Defines the Game Manager Functions			  /
----------------------------------------------------------/
----- Author: Alex Kitching								  /
----------------------------------------------------------/
*/

#include "game_manager.h"

// Loads in object data from the loader before loading in backgrounds and eventually setting up the player.
extern void InitGame(GameManager * const a_pManager)
{
	LoadData(&a_pManager->loader);
	LoadBackgroundData(&a_pManager->bgManager);
	SetupPlayer(&a_pManager->player, &a_pManager->loader.aoObjArray[PLAYER_ID], &a_pManager->loader.aoObjArray[FLAME_ID], &a_pManager->bgManager);
}

// Update Game updates all the objects as well as checking for playerInput.
// It also handles whether the player should move or whether the background should scroll.
extern void UpdateGame(GameManager* const a_pManager)
{
	// Player is Alive
	if (a_pManager->player.uiLives != 0)
	{
		// Retrieve input from Player and update Animation
		Vector playerInput = GetPlayerInput(&a_pManager->player, &a_pManager->bgManager);

		// Check whether we attempt to scroll the map or we just update the player
		if (PlayerHasControl(a_pManager->bgManager.vFGMapPos.x, a_pManager->player.poMyObject->vPosition.x, playerInput))
		{
			//Move Player
			UpdatePlayer(&a_pManager->player, playerInput);
		}
		else
		{
			// Player at a Vertical Map Bound
			if (a_pManager->bgManager.bPlayerAtTopBound | a_pManager->bgManager.bPlayerAtBottomBound)
			{
				// Jump Player
				HandlePlayerYBoundJump(&a_pManager->player, playerInput);

				// Player Input now 0 - Don't Want to Scroll the Background!
				playerInput.y = 0;

				// If Player is no longer at Bound
				if (a_pManager->bgManager.bPlayerAtTopBound && a_pManager->player.poMyObject->vPosition.y >= a_pManager->bgManager.fixPreBoundPlayerYPos)
				{
					a_pManager->bgManager.bPlayerAtTopBound = false;
				}
				else if (a_pManager->bgManager.bPlayerAtBottomBound && a_pManager->player.poMyObject->vPosition.y <= a_pManager->bgManager.fixPreBoundPlayerYPos)
				{
					a_pManager->bgManager.bPlayerAtBottomBound = false;
				}
			}

			// If We Can Scroll
			if (AttemptScrolling(&a_pManager->bgManager, playerInput, a_pManager->player.poMyObject->vPosition.y))
			{
				ScrollForeground(&a_pManager->bgManager, playerInput);
			}
		}
	}
	else // Player is Dead
	{
		if (keyDown(START)) // Restart Game if Start Button is Pressed
		{
			// Reset Player Lives
			a_pManager->player.uiLives = PLAYER_START_LIVES;

			// Unhide the Player
			obj_unhide(a_pManager->player.poMyObject->poaAttrib, A0_MODE_REG);

			// Reset Player and Level
			ResetPlayer(&a_pManager->player);
			ResetLevel(&a_pManager->bgManager, a_pManager->player.pcsMapSnap);
		}
	}

	// Update Snow
	UpdateSnow(&a_pManager->bgManager);
}

// Jumps the Player Sprite Vertically in Screen Space.
void HandlePlayerYBoundJump(Player * const a_pPlayer, const Vector a_vInputVelocity)
{
	// Moves Player Vertically
	Vector VertInput;
	VertInput.y = a_vInputVelocity.y;
	VertInput.x = 0;
	UpdatePlayer(a_pPlayer, VertInput);
}

// PlayerHasControl is used to determine whether whether we move the player or not.
bool PlayerHasControl(const fixed a_fixMapPosX, const fixed a_fixPlayerX, const Vector a_vPlayerInput)
{
	// Get our players future position
	fixed playersFutureX = fixAdd(a_fixPlayerX, a_vPlayerInput.x);
	if (a_fixMapPosX == FG_LEFT_X_BOUND) // We are within left side of the map
	{
		// Players future position is Less than half screen width
		if (fixAdd(playersFutureX, int2fix(PLAYER_DIMENSION >> 1)) < int2fix(SCREEN_W >> 1))
		{
			return true;
		}
		else // Scroll Map Instead
		{
			return false;
		}
	}
	else if (a_fixMapPosX >= int2fix(FG_RIGHT_X_BOUND)) // We are at the right hand side of the map
	{
		// Players future position is greater than half screen width
		if (fixAdd(playersFutureX, int2fix(PLAYER_DIMENSION >> 1)) > int2fix(SCREEN_W >> 1))
		{
			return true;
		}
		else // Scroll Map Instead
		{
			return false;
		}
	}
	else // We are within center of map
	{
		return false;
	}
}
