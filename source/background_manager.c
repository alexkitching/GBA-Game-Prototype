/*
----------------------------------------------------------/
----- Project: GBA Assignment							  /
----- File:    background_manager.c						  /
----------------------------------------------------------/
----- Desc: Defines the Background Manager Functions	  /
----------------------------------------------------------/
----- Author: Alex Kitching								  /
----------------------------------------------------------/
*/

#include "background_manager.h"
#include "BG_Palette.h"
#include "FG_Palette.h"
#include "BG_Map.h"
#include "FG_Map.h"
#include "BG_Tiles.h"
#include "FG_Tiles.h"
#include "FG_Collisions.h"

// Loads Foreground and Background, Initialises Defaults for the Background Manager and Initialises the Snow.
extern void LoadBackgroundData(BackgroundManager * const a_pBackgroundManager)
{
	//Load Backgrounds
	LoadBackgrounds(a_pBackgroundManager);

	// Initialise Defaults
	a_pBackgroundManager->vFGMapPos.x = 0;
	a_pBackgroundManager->vFGMapPos.y = 0;
	a_pBackgroundManager->vNetOffSet.x = 0;
	a_pBackgroundManager->vNetOffSet.y = 0;
	a_pBackgroundManager->vPrevOffSet.x = 0;
	a_pBackgroundManager->vPrevOffSet.y = 0;
	a_pBackgroundManager->siScreenTilePosX = 0;
	a_pBackgroundManager->siScreenTilePosY = 0;
	a_pBackgroundManager->uiFGMapOffSet = BG_TILE_BOUND;
	a_pBackgroundManager->bPlayerAtTopBound = false;
	a_pBackgroundManager->bPlayerAtBottomBound = false;

	// Initialise Snow
	InitialiseSnow(a_pBackgroundManager);
}

// Resets the foreground and background values from a Map Snapshot
void ResetLevel(BackgroundManager * const a_pBackgroundManager, const MapCheckpointSnapShot a_snapShot)
{
	// Load Background Variables from Checkpoint Snapshot
	a_pBackgroundManager->vFGMapPos.x = a_snapShot.vFGMapPos.x;
	a_pBackgroundManager->vFGMapPos.y = a_snapShot.vFGMapPos.y;
	a_pBackgroundManager->vNetOffSet.x = a_snapShot.vNetOffs.x;
	a_pBackgroundManager->vNetOffSet.y = a_snapShot.vNetOffs.y;
	a_pBackgroundManager->vPrevOffSet.x = a_snapShot.vPrevOffs.x;
	a_pBackgroundManager->vPrevOffSet.y = a_snapShot.vPrevOffs.y;
	a_pBackgroundManager->siScreenTilePosX = a_snapShot.siScreenTilePosX;
	a_pBackgroundManager->siScreenTilePosY = a_snapShot.siScreenTilePosY;
	a_pBackgroundManager->uiFGMapOffSet = a_snapShot.uiFGMapOffSet;

	// Reset Bound Bools
	a_pBackgroundManager->bPlayerAtTopBound = false;
	a_pBackgroundManager->bPlayerAtBottomBound = false;

	// Adjust Screen
	REG_BG_OFS[FG_ID].x = fix2int(a_snapShot.vFGMapPos.x);
	REG_BG_OFS[FG_ID].y = fix2int(a_snapShot.vFGMapPos.y);
	REG_BG_OFS[BG_ID].x = fix2int(a_snapShot.vFGMapPos.x);
	REG_BG_OFS[BG_ID].y = fix2int(a_snapShot.vFGMapPos.y);

	// Reload Foreground
	u32 offs = a_pBackgroundManager->uiFGMapOffSet - BG_TILE_BOUND;
	for (u8 row = 0; row < BG_TILE_BOUND; ++row)
	{
		for (u8 col = 0; col < BG_TILE_BOUND; ++col)
		{
			a_pBackgroundManager->puiFGMapMemLoc[row * BG_TILE_BOUND + col] = fg_map[offs + col];
			a_pBackgroundManager->aucCollisionTiles[row * BG_TILE_BOUND + col] = fgCollision[offs + col];
		}
		offs += FG_WIDTH_TILES;
	}

	// Initialise Snow
	InitialiseSnow(a_pBackgroundManager);
}

// Loads Foreground and Background as well as foreground Collision tiles via DMA.
void LoadBackgrounds(BackgroundManager * const a_pBackgroundManager)
{
	// Load BG and FG Tiles, Pallete and Map into Memory via DMA
	dma3_cpy(palette_bg_block_address(FG_ID), fg_Palette, fg_palLen);
	dma3_cpy(palette_bg_block_address(BG_ID), bg_Palette, bg_palLen);
	dma3_cpy(tile_block_address(FG_ID), fg_tiles, fg_tilesLen);
	dma3_cpy(tile_block_address(BG_ID), bg_tiles, bg_tilesLen);

	// Load Foreground
	a_pBackgroundManager->puiFGMapMemLoc = tileMap_block_address(FG_MAPLOC);

	for (u8 row = 0; row < BG_TILE_BOUND; ++row)
	{
		// Copy Map row from Foreground Map
		dma3_cpy(&a_pBackgroundManager->puiFGMapMemLoc[row * BG_TILE_BOUND], &fg_map[row * FG_WIDTH_TILES], sizeof(u16) * BG_TILE_BOUND);

		// Setup Collision Tiles
		for (u8 col = 0; col < BG_TILE_BOUND; col+=2)
		{
			a_pBackgroundManager->aucCollisionTiles[row * BG_TILE_BOUND + col]       = fgCollision[(row * FG_WIDTH_TILES) + col];
			a_pBackgroundManager->aucCollisionTiles[row * BG_TILE_BOUND + (col + 1)] = fgCollision[(row * FG_WIDTH_TILES) + (col + 1)];
		}
	}
	// Set BG Register
	setBG_Control_Register(FG_ID, FG_PRIORITY, FG_TILE_BLOCK_ID, 0, 0, FG_MAPLOC, 0, BG_REG_SIZE_32x32);

	// Load Background
	a_pBackgroundManager->puiBGMapMemLoc = tileMap_block_address(BG_MAPLOC);
	for (u8 row = 0; row < BG_TILE_BOUND; ++row)
	{
		// Copy Map row from Background Map
		dma3_cpy(&a_pBackgroundManager->puiBGMapMemLoc[row * BG_TILE_BOUND], &bg_map[row * BG_WIDTHTILES], sizeof(u16) * BG_TILE_BOUND);
	}
	// Set BG Register
	setBG_Control_Register(BG_ID, BG_PRIORITY, BG_TILE_BLOCK_ID, 0, 0, BG_MAPLOC, 0, BG_REG_SIZE_32x32);
}

// Determines whether we can Scroll the map based on whether the player is at a top or bottom map boundary.
bool AttemptScrolling(BackgroundManager * const a_pBackgroundManager, const Vector a_vInputVelocity, const fixed a_fixPlayerYPos)
{
	if (fixAdd(a_pBackgroundManager->vFGMapPos.y, a_vInputVelocity.y) < 0) // Player is at top of the map
	{
		// Player is at the top Boundary
		a_pBackgroundManager->bPlayerAtTopBound = true;

		// Assign Prebound Y
		a_pBackgroundManager->fixPreBoundPlayerYPos = a_fixPlayerYPos;

		return false; // Can't Scroll
	}
	else if (fixAdd(a_pBackgroundManager->vFGMapPos.y, a_vInputVelocity.y) > int2fix(FG_MIN_HEIGHT)) // Player is at Bottom of Map
	{
		// Player is at the bottom Boundary
		a_pBackgroundManager->bPlayerAtBottomBound = true;

		// Assign Prebound Y
		a_pBackgroundManager->fixPreBoundPlayerYPos = a_fixPlayerYPos;
		return false; // Can't Scroll
	}
	else
	{
		return true; // We can Scroll the Map
	}
}

// Scrolls the Background and Foreground based on players Input.
void ScrollForeground(BackgroundManager * const a_pBackgroundManager, Vector a_vVelocity)
{
	// Save our input Velocity (used for applying to snowflakes)
	a_pBackgroundManager->vInputVelocity = a_vVelocity;
	if (a_vVelocity.x != 0) // We are attempting to move Horizontally
	{
		if (fixAdd(a_pBackgroundManager->vFGMapPos.x, a_vVelocity.x) < 0) // New Velocity would take us out of bounds
		{
			a_vVelocity.x -= fixAdd(a_pBackgroundManager->vFGMapPos.x, a_vVelocity.x); // Adjust Velocity to keep us in bounds
		}

		// Adjust Map Position
		a_pBackgroundManager->vFGMapPos.x += a_vVelocity.x;
		// Adjust Net Offset
		a_pBackgroundManager->vNetOffSet.x += a_vVelocity.x;

		// Adjust Screen X
		REG_BG_OFS[FG_ID].x = fix2int(a_pBackgroundManager->vFGMapPos.x);
		REG_BG_OFS[BG_ID].x = fix2int(fixMul(a_pBackgroundManager->vFGMapPos.x, float2fix(0.2)));
		
		if (a_pBackgroundManager->vPrevOffSet.x == 0) // Haven't moved yet
		{
			// Set our new Offset
			a_pBackgroundManager->vPrevOffSet.x = a_vVelocity.x;
		}
		else if (a_pBackgroundManager->vPrevOffSet.x > 0 && a_vVelocity.x < 0) // Previously Moved Right now Moving left
		{
			// Set our new Offset
			a_pBackgroundManager->vPrevOffSet.x = a_vVelocity.x;
			// Immediately load left tiles
			LoadLeftTiles(a_pBackgroundManager);
		}
		else if (a_pBackgroundManager->vPrevOffSet.x < 0 && a_vVelocity.x > 0) // Previously Moved Left now Moving Right
		{
			// Set our new Offset
			a_pBackgroundManager->vPrevOffSet.x = a_vVelocity.x;
			// Immediately load right tiles
			LoadRightTiles(a_pBackgroundManager);
		}

		// We have traversed one tile right
		if (a_pBackgroundManager->vNetOffSet.x > int2fix(TILE_DIMENSION))
		{
			// Reduce our NeT Offset by Tile Dimension
			a_pBackgroundManager->vNetOffSet.x -= int2fix(TILE_DIMENSION);
			// Our Screen will have travelled 1 tile over
			a_pBackgroundManager->siScreenTilePosX = (a_pBackgroundManager->siScreenTilePosX + 1) % BG_TILE_BOUND;
			// Load our right tiles
			LoadRightTiles(a_pBackgroundManager);
		}// Else we have traversed one tile left
		else if (a_pBackgroundManager->vNetOffSet.x < int2fix(-TILE_DIMENSION))
		{
			// Increase our Net Offset by Tile Dimension
			a_pBackgroundManager->vNetOffSet.x += int2fix(TILE_DIMENSION);
			// Our Screen will have travelled 1 tile over
			a_pBackgroundManager->siScreenTilePosX = (a_pBackgroundManager->siScreenTilePosX - 1) % BG_TILE_BOUND;
			// Load our left tiles
			LoadLeftTiles(a_pBackgroundManager);
		}
	}

	if (a_vVelocity.y != 0) // Y Velocity Present
	{
		// Adjust Map Position
		a_pBackgroundManager->vFGMapPos.y += a_vVelocity.y;

		// Adjust Screen Y
		REG_BG_OFS[FG_ID].y = fix2int(a_pBackgroundManager->vFGMapPos.y);
		REG_BG_OFS[BG_ID].y = fix2int(fixMul(a_pBackgroundManager->vFGMapPos.y, float2fix(0.2)));
	}
}

// Initialises Snow Particles.
void InitialiseSnow(BackgroundManager * const a_pBackgroundManager)
{
	// Create Default Snow Attribute
	ObjectAttribute snowAttrib;
	snowAttrib.attr0 = SetAttrib0(0, A0_MODE_REG, 0, 0, A0_4BPP, A0_SQUARE);
	snowAttrib.attr1 = SetAttrib1(0, 0, A1_SIZE_0);
	snowAttrib.pad = 0;

	// Assign OAM
	a_pBackgroundManager->poaSnowOAM = &MEM_OAM[2];

	// Loop and Setup Each Particle
	for (int i = 0; i < SNOWFLAKE_COUNT; i++)
	{
		// Assign Particle Start Position and Velocity
		Vector newPosition;
		newPosition.x = int2fix(qran_range(0, SCREEN_W));
		newPosition.y = int2fix(qran_range(0, SCREEN_H));

		Vector newVelocity;
		newVelocity.x = int2fix(qran_range(SNOWFLAKE_VXMIN, SNOWFLAKE_VXMAX));
		newVelocity.y = fixMul(int2fix(qran_range(SNOWFLAKE_VYMIN, SNOWFLAKE_VYMAX)), float2fix(SNOWFLAKE_VYMULTIPLIER));

		// Initialise Particle
		InitParticle(&a_pBackgroundManager->apartSnowflakes[i], newPosition, newVelocity, SNOWFLAKE_FRAMES, GRAVITY);
		
		// Half Particles assign to Background Half to Foreground
		if (i % 2 == 1)
		{
			snowAttrib.attr2 = SetAttrib2(24, SNOWFLAKE_BG_PRIORITY, 2);
		}
		else
		{
			snowAttrib.attr2 = SetAttrib2(24, SNOWFLAKE_FG_PRIORITY, 2);
		}

		// Assign Attribute 
		a_pBackgroundManager->poaSnowOAM[i] = snowAttrib;
	}
}

// Updates Snow Particles.
void UpdateSnow(BackgroundManager * const a_pBackgroundManager)
{
	// Loop through each Snowflake
	for (int i = 0; i < SNOWFLAKE_COUNT; i++)
	{
		// Get a Pointer to our current SnowFlake
		Particle* currentSnowflake = &a_pBackgroundManager->apartSnowflakes[i];

		// Update Velocity relative to Scrolling Background Velocity
		currentSnowflake->vPosition.x = fixAdd(currentSnowflake->vPosition.x, fixSub(currentSnowflake->vVelocity.x, a_pBackgroundManager->vInputVelocity.x));
		currentSnowflake->vPosition.y = fixAdd(currentSnowflake->vPosition.y, fixSub(currentSnowflake->vVelocity.y, a_pBackgroundManager->vInputVelocity.y));

		// Snowflake has moved off screen
		if (currentSnowflake->vPosition.y > int2fix(SCREEN_H))
		{
			// Assign new Position
			currentSnowflake->vPosition.x = int2fix(qran_range(0, SCREEN_W));
			currentSnowflake->vPosition.y = int2fix(-SNOWFLAKE_DIMENSION);

			// Assign new Velocity
			currentSnowflake->vVelocity.x = int2fix(qran_range(SNOWFLAKE_VXMIN, SNOWFLAKE_VXMAX));
			currentSnowflake->vVelocity.y = fixMul(int2fix(qran_range(SNOWFLAKE_VYMIN, SNOWFLAKE_VYMAX)), float2fix(SNOWFLAKE_VYMULTIPLIER));

			// Reset Particle
			InitParticle(&a_pBackgroundManager->apartSnowflakes[i], currentSnowflake->vPosition, currentSnowflake->vVelocity, SNOWFLAKE_FRAMES, GRAVITY);
		}

		// Move Particle
		SetObjectPosition(&a_pBackgroundManager->poaSnowOAM[i], fix2int(currentSnowflake->vPosition.x), fix2int(currentSnowflake->vPosition.y));
	}
}

// Loads Column of Tiles at Left Hand Side of Screen
void LoadLeftTiles(BackgroundManager * const a_pBackgroundManager)
{
	// Get Starting Column
	s16 newTileCol = a_pBackgroundManager->siScreenTilePosX - 1;

	// Wrap around if less than 0
	if (newTileCol < 0)
	{
		newTileCol += BG_TILE_BOUND;
	}

	// Get our Starting offset
	u32 mapOffs = a_pBackgroundManager->uiFGMapOffSet - BG_TILE_BOUND - 1;

	for (u8 row = 0; row < BG_TILE_BOUND; row +=2)
	{
		// Set our Tile
		a_pBackgroundManager->puiFGMapMemLoc[newTileCol + (row * BG_TILE_BOUND)] = fg_map[mapOffs + (row * FG_WIDTH_TILES)];
		// Set our Collision Tile
		a_pBackgroundManager->aucCollisionTiles[newTileCol + (row * BG_TILE_BOUND)] = fgCollision[mapOffs + (row * FG_WIDTH_TILES)];
	}

	// Decrease our managers map offset by 1 Column
	a_pBackgroundManager->uiFGMapOffSet -= 1;
}

// Loads Column of Tiles at Right Hand Side of Screen
void LoadRightTiles(BackgroundManager * const a_pBackgroundManager)
{
	// Get Starting Column
	s16 newTileCol = a_pBackgroundManager->siScreenTilePosX + BG_TILE_BOUND - 1;

	// Wrap around if less than 0
	if (newTileCol > 31)
	{
		newTileCol -= BG_TILE_BOUND;
	}

	// Get our Starting offset
	u32 mapOffs = a_pBackgroundManager->uiFGMapOffSet;

	for (u8 row = 0; row < BG_TILE_BOUND; ++row)
	{
		// Set our Tile
		a_pBackgroundManager->puiFGMapMemLoc[newTileCol + (row * BG_TILE_BOUND)] = fg_map[mapOffs + (row * FG_WIDTH_TILES)];
		// Set our Collision Tile
		a_pBackgroundManager->aucCollisionTiles[newTileCol + (row * BG_TILE_BOUND)] = fgCollision[mapOffs + (row * FG_WIDTH_TILES)];
	}

	// Increase our managers map offset by 1 Column
	a_pBackgroundManager->uiFGMapOffSet += 1;
}