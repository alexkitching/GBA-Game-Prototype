#ifndef __BACKGROUND_MANAGER_H__
#define __BACKGROUND_MANAGER_H__

/*
----------------------------------------------------------/
----- Project: GBA Assignment							  /
----- File:    background_manager.h						  /
----------------------------------------------------------/
----- Desc: Declares the Background Manager Struct,		  /
-----       Map Checkpoint Snapshot Struct and Functions, /
-----		Background Manager Handles Background		  /
-----       Scrolling and Loading. Map Checkpoint Snapshot/
-----       is used to save current Map Variables for	  /
-----       reset upon player death.					  /
----------------------------------------------------------/
----- Author: Alex Kitching								  /
----------------------------------------------------------/
*/

#include "gba.h"

// Gravity -9.8mps
#define GRAVITY 0x9CC

// Foreground Defines
#define FG_ID 0
#define FG_PRIORITY 0
#define FG_TILE_BLOCK_ID 0
#define FG_MAPLOC 16
#define FG_WIDTH_TILES 128
#define FG_MIN_HEIGHT 96
#define FG_LEFT_X_BOUND 0
#define FG_RIGHT_X_BOUND 784

// Background Defines
#define BG_ID 1
#define BG_PRIORITY 1
#define BG_TILE_BLOCK_ID 1
#define BG_MAPLOC 17
#define BG_WIDTHTILES 32

// General Defines
#define TILE_DIMENSION 8
#define BG_TILE_BOUND 32

// Snow Defines
#define SNOWFLAKE_DIMENSION 8
#define SNOWFLAKE_COUNT 32
#define SNOWFLAKE_VXMIN 3
#define SNOWFLAKE_VXMAX 5
#define SNOWFLAKE_VYMIN 1
#define SNOWFLAKE_VYMAX 4
#define SNOWFLAKE_VYMULTIPLIER 0.4
#define SNOWFLAKE_FRAMES 512
#define SNOWFLAKE_FG_PRIORITY 4
#define SNOWFLAKE_BG_PRIORITY 3

// Collision Defines
#define DeathTile 0x0003
#define FreeTile  0x0000
#define RightSlopeTile 0x0001
#define LeftSlopeTile 0x0002
#define BlockTile 0x0004
#define CheckpointTile 0x0008

typedef struct MapCheckpointSnapShot
{
	// Map Variables
	Vector vFGMapPos;
	Vector vNetOffs;
	Vector vPrevOffs;
	u32 uiFGMapOffSet;
	// Screen Variables
	s8 siScreenTilePosX;
	s8 siScreenTilePosY;
	// Padding
	u8 pad[2];
} MapCheckpointSnapShot; // 32 Bytes

typedef struct BackgroundManager
{
	// Foreground Variables
	u16* puiFGMapMemLoc; 
	u32 uiFGMapOffSet; 
	Vector vFGMapPos;  
	unsigned char aucCollisionTiles[1024]; // 32x32 Tiles

	// Background Variables
	u16* puiBGMapMemLoc; 

	// Scrolling Variables
	Vector vInputVelocity; 
	Vector vNetOffSet; 
	Vector vPrevOffSet; 
	s8 siScreenTilePosX; 
	s8 siScreenTilePosY; 

	// Vertical Bounds bools
	bool bPlayerAtTopBound; 
	bool bPlayerAtBottomBound; 
	fixed fixPreBoundPlayerYPos; 

	// Snow Particle System
	Particle apartSnowflakes[SNOWFLAKE_COUNT];
	ObjectAttribute* poaSnowOAM; 
}BackgroundManager; // 2104 Bytes

extern void LoadBackgroundData(BackgroundManager * const a_pBackgroundManager);
extern void ResetLevel(BackgroundManager * const a_pBackgroundManager, const MapCheckpointSnapShot a_snapShot);
void LoadBackgrounds(BackgroundManager * const a_pBackgroundManager);
extern bool AttemptScrolling(BackgroundManager * const a_pBackgroundManager, const Vector a_vInputVelocity, const fixed a_fixPlayerYPos);
extern void ScrollForeground(BackgroundManager * const a_pBackgroundManager, Vector a_vVelocity);
void InitialiseSnow(BackgroundManager * const a_pBackgroundManager);
extern void UpdateSnow(BackgroundManager * const a_pBackgroundManager);
void LoadLeftTiles(BackgroundManager * const a_pBackgroundManager);
void LoadRightTiles(BackgroundManager * const a_pBackgroundManager);
#endif // !__BACKGROUND_MANAGER_H__
