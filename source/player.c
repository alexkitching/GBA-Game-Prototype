/*
----------------------------------------------------------/
----- Project: GBA Assignment							  /
----- File:    player.c									  /
----------------------------------------------------------/
----- Desc: Defines the Player Functions				  /
----------------------------------------------------------/
----- Author: Alex Kitching								  /
----------------------------------------------------------/
*/

#include "player.h"
#include "loader.h"
#include "sorlo.h"

// Initialises Player Variables and assigns objects
extern void SetupPlayer(Player * const a_player, Object * const a_playerObject, Object * const a_poFlameObject, BackgroundManager * const a_pBackgroundManager)
{
	a_player->poMyObject = a_playerObject;
	a_player->psCurrentState = STANDING;
	a_player->bMoveDir = NULL;
	a_player->bGrounded = false;
	a_player->bCheckPointReached = false;
	a_player->uiLives = PLAYER_START_LIVES;
	a_player->poMyObject->vVelocity.x = 0;
	a_player->poMyObject->vVelocity.y = 0;
	a_player->vMapPos.x = a_playerObject->vPosition.x;
	a_player->vMapPos.y = a_playerObject->vPosition.y;

	// Animation Setup
	SetupAnimController(&a_player->oacMyAnimController, a_playerObject, int2fix(PLAYER_MOVING_ANIM_TIME));
	// Add Static Animation
	a_player->uiStaticAnimID = AddStaticAnim(&a_player->oacMyAnimController, sorlostaticPal, sorlostaticPalLen, sorlostaticTiles, sorlostaticTilesLen);
	// Add Jumping Animation
	a_player->uiJumpAnimID = AddStaticAnim(&a_player->oacMyAnimController, sorlojumpPal, sorlojumpPalLen, sorlojumpTiles, sorlojumpTilesLen);
	// Add Attack Animation
	a_player->uiAttackAnimID = AddStaticAnim(&a_player->oacMyAnimController, sorloattackPal, sorloattackPalLen, sorloattackTiles, sorloattackTilesLen);
	// Add Moving Animation
	AddDynamicAnim(&a_player->oacMyAnimController, sorlowalk1Pal, sorlowalk1PalLen, sorlowalk1Tiles, sorlowalk1TilesLen);
	AddDynamicAnim(&a_player->oacMyAnimController, sorlowalk2Pal, sorlowalk2PalLen, sorlowalk2Tiles, sorlowalk2TilesLen);
	
	// Setup Flame Attack
	SetupFlame(&a_player->pfMyFlame, a_poFlameObject);

	// Setup Initial Checkpoint
	SaveCheckPointSnapShot(a_player, a_pBackgroundManager);
}

// Handles Player Input, calculates velocity, checks collision before returning a Vector Output
extern Vector GetPlayerInput(Player * const a_player, BackgroundManager * const a_pBackgroundManager)
{
	switch (a_player->psCurrentState)
	{
	case STANDING:
		// Player not currently in Standing Anim
		if (a_player->oacMyAnimController.siCurrentAnimID != a_player->uiStaticAnimID)
		{
			// Load Standing Anim
			LoadAnim(&a_player->oacMyAnimController, a_player->uiStaticAnimID);
		}

		// Player is not on the ground
		if (!a_player->bGrounded)
		{
			a_player->psCurrentState = JUMPING; 
			break;
		}

		if (keyDown(LEFT))
		{
			a_player->bMoveDir = MOVE_LEFT;
			a_player->psCurrentState = MOVING;
			break;
		}
		else if (keyDown(RIGHT))
		{
			a_player->bMoveDir = MOVE_RIGHT;
			a_player->psCurrentState = MOVING;
			break;
		}
		else if (keyDown(A))
		{
			a_player->psCurrentState = ATTACKING;
			break;
		}
		else // No Input
		{
			ApplyHorizontalDrag(a_player);
		}

		if (keyDown(UP))
		{
			Jump(a_player);
			break;
		}
		break;
	case MOVING:
		// Player is not on the ground
		if (!a_player->bGrounded)
		{
			a_player->psCurrentState = JUMPING;
			break;
		}
		else if (!keyDown(LEFT) && !keyDown(RIGHT))
		{
			a_player->psCurrentState = STANDING;
		}
		else if (keyDown(A))
		{
			a_player->psCurrentState = ATTACKING;
		}

		Move(a_player);

		if (keyDown(UP))
		{
			Jump(a_player);
			break;
		}

		// Play Moving Anim
		PlayDynamicAnim(&a_player->oacMyAnimController);
		break;
	case JUMPING:
		if (a_player->bGrounded) // We are on the ground
		{
			a_player->psCurrentState = STANDING;
		}

		// Not currently in Jump Anim
		if (a_player->oacMyAnimController.siCurrentAnimID != a_player->uiJumpAnimID)
		{
			LoadAnim(&a_player->oacMyAnimController, a_player->uiJumpAnimID);
		}
		break;
	case ATTACKING:
		// Player is not on the ground
		if (!a_player->bGrounded)
		{
			// No Longer Casting Flame
			EndFlame(&a_player->pfMyFlame);
			a_player->psCurrentState = JUMPING;
			break;
		}

		// Not currently in Attacking Anim
		if (a_player->oacMyAnimController.siCurrentAnimID != a_player->uiAttackAnimID)
		{
			LoadAnim(&a_player->oacMyAnimController, a_player->uiAttackAnimID);
		}

		if (!keyDown(A))
		{
			// No Longer attacking
			EndFlame(&a_player->pfMyFlame);
			a_player->psCurrentState = STANDING;
			break;
		}

		if (a_player->poMyObject->vVelocity.x != 0) // Player is currently moving
		{
			ApplyHorizontalDrag(a_player);
		}

		// Calculate Flame Position based on Player position and facing direction
		Vector vFlamePos;
		vFlamePos.y = fixAdd(a_player->poMyObject->vPosition.y, int2fix(FLAME_YOFFS));
		if (a_player->poMyObject->bFacingDirection == RIGHTFACE)
		{
			vFlamePos.x = fixAdd(a_player->poMyObject->vPosition.x, int2fix(BG_TILE_BOUND));
		}
		else if (a_player->poMyObject->bFacingDirection == LEFTFACE)
		{
			vFlamePos.x = fixSub(a_player->poMyObject->vPosition.x, int2fix(BG_TILE_BOUND));
		}

		// Update Flame using Calculated Position
		UpdateFlame(&a_player->pfMyFlame, vFlamePos, a_player->poMyObject->bFacingDirection);
		break;
	}

	// Apply Gravity
	a_player->poMyObject->vVelocity.y += fixMul(GRAVITY, FRAMETIME);

	// Get Player Input
	Vector vPlayerInput;
	vPlayerInput.x = fixMul(fixMul(a_player->poMyObject->vVelocity.x, FRAMETIME), PIXELS2METERS);
	vPlayerInput.y = fixMul(fixMul(a_player->poMyObject->vVelocity.y, FRAMETIME), PIXELS2METERS);

	// Check Collisions
	vPlayerInput = CheckCollisions(a_player, a_pBackgroundManager, vPlayerInput);

	// Adjust Players Map Positon
	if (vPlayerInput.x != 0)
		a_player->vMapPos.x += vPlayerInput.x;
	if (vPlayerInput.y != 0)
		a_player->vMapPos.y += vPlayerInput.y;

	return vPlayerInput;
}

// Adjusts Velocity based on Movement Direction
void Move(Player * const a_player)
{
	if (a_player->bMoveDir == MOVE_LEFT)
	{
		if (a_player->poMyObject->vVelocity.x > int2fix(-3)) // Max Velocity not reached
		{
			a_player->poMyObject->vVelocity.x -= PLAYER_ACCELERATION; // Increase Velocity
		}
		if (a_player->poMyObject->bFacingDirection == RIGHTFACE) // Swap Facing Direction if required
		{
			a_player->poMyObject->bFacingDirection = LEFTFACE;
			FlipObjectHorizontal(a_player->poMyObject);
		}
	}
	else if (a_player->bMoveDir == MOVE_RIGHT)
	{
		if (a_player->poMyObject->vVelocity.x < int2fix(3)) // Max Velocity not reached
		{
			a_player->poMyObject->vVelocity.x += PLAYER_ACCELERATION; // Increase Velocity
		}
		if (a_player->poMyObject->bFacingDirection == LEFTFACE) // Swap Facing Direction if required
		{
			a_player->poMyObject->bFacingDirection = RIGHTFACE;
			FlipObjectHorizontal(a_player->poMyObject);
		}
	}
}

// Applys Drag based on X Velocity
void ApplyHorizontalDrag(Player * const a_player)
{
	if (a_player->poMyObject->vVelocity.x < 0) // Moving Left
	{
		a_player->poMyObject->vVelocity.x += PLAYER_ACCELERATION; // Apply Drag
	}
	else if (a_player->poMyObject->vVelocity.x > 0) // Moving Right
	{
		a_player->poMyObject->vVelocity.x -= PLAYER_ACCELERATION; // Apply Drag
	}
}

// Jumps the Player
void Jump(Player * const a_player)
{
	a_player->psCurrentState = JUMPING;
	// Now Jumping
	a_player->bGrounded = false;
	// Change Vertical Velocity
	a_player->poMyObject->vVelocity.y = int2fix(PLAYER_JUMP_FORCE);
	// Load in Jumping Sprite
	LoadAnim(&a_player->oacMyAnimController, a_player->uiJumpAnimID);
}

// Resets the player to his last Checkpoint Snapshot
void ResetPlayer(Player * const a_player)
{
	// Load Player Variables from Checkpoint SnapShot
	a_player->poMyObject->vPosition.x = a_player->pcsMySnap.vPlayerPos.x;
	a_player->poMyObject->vPosition.y = a_player->pcsMySnap.vPlayerPos.y;
	a_player->psCurrentState = a_player->pcsMySnap.psCurrentState;
	a_player->bGrounded = a_player->pcsMySnap.bGrounded;
	a_player->vMapPos.x = a_player->pcsMySnap.vMapPos.x;
	a_player->vMapPos.y = a_player->pcsMySnap.vMapPos.y;

	// Set Velocity to 0
	a_player->poMyObject->vVelocity.x = 0;
	a_player->poMyObject->vVelocity.y = 0;

	// Move our Player
	SetObjectPosition(a_player->poMyObject->poaAttrib, a_player->poMyObject->vPosition.x, a_player->poMyObject->vPosition.y);
}

// Checks Collisions using Players Future position and background managers collision tiles.
Vector CheckCollisions(Player * const a_player, BackgroundManager * const a_pBackgroundManager, Vector a_vPlayerInput)
{
	// Input to Return
	Vector inputToReturn = a_vPlayerInput;
	// Check Player Position 
	Vector vNewPlayerPos;
	vNewPlayerPos.x = fixAdd(a_player->vMapPos.x, a_vPlayerInput.x);
	vNewPlayerPos.y = fixAdd(a_player->vMapPos.y, a_vPlayerInput.y);

	// Get Player Tile Pos
	u8 uiPlayerTilePosX = fix2int(vNewPlayerPos.x >> 3) % BG_TILE_BOUND;
	u8 uiPlayerTilePosY = fix2int(vNewPlayerPos.y >> 3) % BG_TILE_BOUND;

	if (a_vPlayerInput.x > 0) // Moving Right
	{
		u8 uiPlayerTopRightBoundTileX =    (uiPlayerTilePosX + (PLAYER_DIMENSION >> 3))       % BG_TILE_BOUND;
		u8 uiPlayerTopRightBoundTileY =     uiPlayerTilePosY;
		u8 uiPlayerBottomRightBoundTileX = (uiPlayerTilePosX + (PLAYER_DIMENSION >> 3))       % BG_TILE_BOUND;
		u8 uiPlayerBottomRightBoundTileY = (uiPlayerTilePosY + ((PLAYER_DIMENSION - 1) >> 3)) % BG_TILE_BOUND;

		// Get Collision Tiles
		unsigned char ucTopRightBoundTile =    a_pBackgroundManager->aucCollisionTiles[uiPlayerTopRightBoundTileY    * BG_TILE_BOUND + uiPlayerTopRightBoundTileX];
		unsigned char ucBottomRightBoundTile = a_pBackgroundManager->aucCollisionTiles[uiPlayerBottomRightBoundTileY * BG_TILE_BOUND + uiPlayerBottomRightBoundTileX];

		// Not a Free, Checkpoint, Right Slope Tile
		if ((ucTopRightBoundTile    != FreeTile			&&
			 ucTopRightBoundTile    != CheckpointTile)	||
			(ucBottomRightBoundTile != FreeTile			&&
			 ucBottomRightBoundTile != RightSlopeTile	&&
			 ucBottomRightBoundTile != CheckpointTile))
		{
			// Stop Moving Right
			inputToReturn.x = 0;
		}
		else if (ucBottomRightBoundTile == RightSlopeTile) // Bottom Right Tile is a Right Slope
		{
			// Move player up
			a_vPlayerInput.y = -a_vPlayerInput.x;
			inputToReturn.y =   a_vPlayerInput.y;
		}

		if (!a_player->bCheckPointReached) // Checkpoint not yet been reached
		{
			if (uiPlayerTopRightBoundTileX == CheckpointTile || // Tile is a checkpoint tile
				ucBottomRightBoundTile     == CheckpointTile)
			{
				/* DOESNT WORK */
				//a_player->CheckPointReached = true;
				//SaveCheckPointSnapShot(a_player, a_pBackgroundManager);
			}
		}

	}
	else if (a_vPlayerInput.x < 0) // Moving Left
	{
		u8 uiPlayerTopLeftBoundTileX =     uiPlayerTilePosX;
		u8 uiPlayerTopLeftBoundTileY =     uiPlayerTilePosY;
		u8 uiPlayerBottomLeftBoundTileX =  uiPlayerTilePosX;
		u8 uiPlayerBottomLeftBoundTileY = (uiPlayerTilePosY + ((PLAYER_DIMENSION - 1) >> 3)) % BG_TILE_BOUND;

		// Get Collision Tiles
		unsigned char ucTopLeftBoundTile = a_pBackgroundManager->aucCollisionTiles[uiPlayerTopLeftBoundTileY       * BG_TILE_BOUND + uiPlayerTopLeftBoundTileX];
		unsigned char ucBottomLeftBoundTile = a_pBackgroundManager->aucCollisionTiles[uiPlayerBottomLeftBoundTileY * BG_TILE_BOUND + uiPlayerBottomLeftBoundTileX];

		// Not a Free, Checkpoint, Left Slope Tile
		if ((ucTopLeftBoundTile	   != FreeTile			&&
			 ucTopLeftBoundTile	   != CheckpointTile)	||
			(ucBottomLeftBoundTile != FreeTile			&&
			 ucBottomLeftBoundTile != LeftSlopeTile		&&
			 ucBottomLeftBoundTile != CheckpointTile))
		{
			// Stop Moving Left
			inputToReturn.x = 0;
		}
		else if (ucBottomLeftBoundTile == LeftSlopeTile) // Bottom Left Tile is a Left Slope
		{
			a_vPlayerInput.y = a_vPlayerInput.x;
			inputToReturn.y = a_vPlayerInput.y;
		}

		if (!a_player->bCheckPointReached) // Checkpoint not yet been reached
		{
			if (ucTopLeftBoundTile    == CheckpointTile ||
				ucBottomLeftBoundTile == CheckpointTile) // Tile is a checkpoint tile
			{
				/* DOESNT WORK */
				//a_player->CheckPointReached = true;
				//SaveCheckPointSnapShot(a_player, a_pBackgroundManager);
			}
		}
	}

	if (a_vPlayerInput.y > 0) // Moving Down
	{
		// Get Tile Position
		u8 uiPlayerBottomTileY =            (uiPlayerTilePosY + (PLAYER_DIMENSION >> 3)) %  BG_TILE_BOUND;
		u8 uiPlayerBottomLeftBoundTileX =    uiPlayerTilePosX;
		u8 uiPlayerBottomMiddleBoundTileX = (uiPlayerTilePosX + ((PLAYER_DIMENSION >> 1) >> 3)) % BG_TILE_BOUND;
		u8 uiPlayerBottomRightBoundTileX =  (uiPlayerTilePosX + (PLAYER_DIMENSION >> 3)) %  BG_TILE_BOUND;

		// Get Collision Tiles
		unsigned char ucBottomLeftBoundTile = a_pBackgroundManager->aucCollisionTiles[uiPlayerBottomTileY   * BG_TILE_BOUND + uiPlayerBottomLeftBoundTileX];
		unsigned char ucBottomMiddleBoundTile = a_pBackgroundManager->aucCollisionTiles[uiPlayerBottomTileY * BG_TILE_BOUND + uiPlayerBottomMiddleBoundTileX];
		unsigned char ucBottomRightBoundTile = a_pBackgroundManager->aucCollisionTiles[uiPlayerBottomTileY  * BG_TILE_BOUND + uiPlayerBottomRightBoundTileX];

		// A Bottom Tile is a Death Tile
		if (ucBottomLeftBoundTile   == DeathTile ||
			ucBottomMiddleBoundTile == DeathTile ||
			ucBottomRightBoundTile  == DeathTile)
		{
			// Reduce Players Lives
			--a_player->uiLives;

			if (a_player->uiLives > 0) // Player Still has Lives
			{
				// Reset Player and Level
				ResetPlayer(a_player);
				ResetLevel(a_pBackgroundManager, a_player->pcsMapSnap);
			}
			else // No Lives Left
			{
				// Hide the Player
				obj_hide(a_player->poMyObject->poaAttrib);
			}
		}	// Not a Free or Checkpoint tile
		else if ((ucBottomLeftBoundTile   != FreeTile			&&
				  ucBottomLeftBoundTile   != CheckpointTile)	||
				 (ucBottomMiddleBoundTile != FreeTile			&&
				  ucBottomMiddleBoundTile != CheckpointTile)	||
				 (ucBottomRightBoundTile  != FreeTile			&&
				  ucBottomRightBoundTile  != CheckpointTile))
		{
			// Stop Moving Down
			a_player->poMyObject->vVelocity.y = 0;
			// Output velocity set to 0
			inputToReturn.y = 0;

			if (!a_player->bGrounded) // Wasn't Previously Grounded
			{
				a_player->bGrounded = true; // Now Grounded
			}
		}
		else if (a_player->bGrounded) // No Tile Below - Player should be no longer grounded
		{
			a_player->bGrounded = false;
		}
	}
	else if (a_vPlayerInput.y < 0) // Moving Up
	{
		// Get Tile Position
		u8 PlayerTopLeftBoundTileX =   uiPlayerTilePosX;
		u8 PlayerTopLeftBoundTileY =   uiPlayerTilePosY;
		u8 PlayerTopRightBoundTileX = (uiPlayerTilePosX + (PLAYER_DIMENSION >> 3)) % BG_TILE_BOUND;
		u8 PlayerTopRightBoundTileY =  uiPlayerTilePosY;

		// Get Collision Tiles
		unsigned char TopLeftBoundTile =  a_pBackgroundManager->aucCollisionTiles[PlayerTopLeftBoundTileY  * BG_TILE_BOUND + PlayerTopLeftBoundTileX];
		unsigned char TopRightBoundTile = a_pBackgroundManager->aucCollisionTiles[PlayerTopRightBoundTileY * BG_TILE_BOUND + PlayerTopRightBoundTileX];

		// Not a Free or Checkpoint tile
		if ((TopLeftBoundTile  != FreeTile			&&
			 TopLeftBoundTile  != CheckpointTile)	||
			(TopRightBoundTile != FreeTile			&&
			 TopRightBoundTile != CheckpointTile))
		{
			// Stop Moving Up - Also Halt Players Current Y Velocity
			a_player->poMyObject->vVelocity.y = 0;
			inputToReturn.y = 0;
		}
	}
	// Return Altered Input Velocity
	return inputToReturn;
}

// Saves Player and Map Variables for a Checkpoint Reset
void SaveCheckPointSnapShot(Player * const a_player, BackgroundManager * const a_pBackgroundManager)
{
	// Save Player Data
	a_player->pcsMySnap.bCheckPointReached = a_player->bCheckPointReached;
	a_player->pcsMySnap.psCurrentState = a_player->psCurrentState;
	a_player->pcsMySnap.bGrounded = a_player->bGrounded;
	a_player->pcsMySnap.vMapPos = a_player->vMapPos;
	a_player->pcsMySnap.vPlayerPos = a_player->poMyObject->vPosition;

	// Save Map Data
	a_player->pcsMapSnap.vFGMapPos = a_pBackgroundManager->vFGMapPos;
	a_player->pcsMapSnap.vNetOffs = a_pBackgroundManager->vNetOffSet;
	a_player->pcsMapSnap.vPrevOffs = a_pBackgroundManager->vPrevOffSet;
	a_player->pcsMapSnap.siScreenTilePosX = a_pBackgroundManager->siScreenTilePosX;
	a_player->pcsMapSnap.siScreenTilePosY = a_pBackgroundManager->siScreenTilePosY;
	a_player->pcsMapSnap.uiFGMapOffSet = a_pBackgroundManager->uiFGMapOffSet;
}
 
// Updates Player using Velocity Parameter
void UpdatePlayer(Player * const a_player, const Vector a_vVelocity)
{
	if (a_vVelocity.x != 0) // X Velocity Present
	{
		a_player->poMyObject->vPosition.x = fixAdd(a_player->poMyObject->vPosition.x, a_vVelocity.x);
	}
	
	if (a_vVelocity.y != 0) // Y Velocity Present
	{
		a_player->poMyObject->vPosition.y = fixAdd(a_player->poMyObject->vPosition.y, a_vVelocity.y);
	}

	SetObjectPosition(a_player->poMyObject->poaAttrib, fix2int(a_player->poMyObject->vPosition.x), fix2int(a_player->poMyObject->vPosition.y));
}