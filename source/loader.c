/*
----------------------------------------------------------/
----- Project: GBA Assignment							  /
----- File:    loader.c									  /
----------------------------------------------------------/
----- Desc: Defines the Loader Functions.				  /
----------------------------------------------------------/
----- Author: Alex Kitching								  /
----------------------------------------------------------/
*/

#include "Intellisense.h"
#include <string.h>
#include "loader.h"
#include "sorlo.h"
#include "snow.h"
#include "flame.h"

// Initialises Loader Vars, OAM buffer and Loads Objects
extern void LoadData(Loader * const a_pLoader)
{
	// Initialise Obj and Particle Count to 0
	a_pLoader->uiObjCount = 0;
	a_pLoader->uiObjallocatedTiles = 0;

	// Initalise OAM Buffer
	oam_init(obj_buffer, MAXOBJ);

	// Load Sorlo Player Object (ID = 0)
	LoadObject(a_pLoader, sorlostaticPal, sorlostaticPalLen, sorlostaticTiles, sorlostaticTilesLen);
	InitialiseObject(&a_pLoader->aoObjArray[a_pLoader->uiObjCount - 1], PLAYER_STARTPOSY, 0, 0, 0, A0_4BPP, A0_SQUARE, PLAYER_STARTPOSX, 0, A1_SIZE_2, PLAYER_PRIORITY, 0, 0, RIGHTFACE);

	// Load Sorlo Flames (ID = 1)
	LoadObject(a_pLoader, flamestart1Pal, flamestart1PalLen, flamestart1Tiles, flamestart1TilesLen);

	// Load Snow Particles (ID = 2)
	LoadObject(a_pLoader, snowPal, snowPalLen, snowTiles, snowTilesLen);
}

// Loads and Sets up an Object using Direct Memory Transfer
void LoadObject(Loader * const a_pLoader, const unsigned short *a_pusPallete, const u16  a_uiPalleteLength, const unsigned short *a_pusTiles, const u16  a_uiTilesLength)
{
	// Current Mem + NewSpriteSizeMem is less than Max Sprite Mem
	if (a_pLoader->uiObjallocatedTiles + (a_uiTilesLength >> 5) < MAXOBJ_TILES && a_pLoader->uiObjCount < MAXOBJ)
	{
		// Load Tiles and Pallete into Memory via DMA
		dma3_cpy(palette_sp_block_address(a_pLoader->uiObjCount), a_pusPallete, a_uiPalleteLength);
		dma3_cpy(sprite_tile_block_address(a_pLoader->uiObjallocatedTiles), a_pusTiles, a_uiTilesLength);

		// Setup a new Object
		SetupObject(a_pLoader, &a_pLoader->aoObjArray[a_pLoader->uiObjCount]);

		// Update Allocated Memory by Shifting Tiles down by 4 Bits to get the number of tiles
		a_pLoader->uiObjallocatedTiles += (a_uiTilesLength >> 5);
		// Increment our obj count
		++a_pLoader->uiObjCount;
	}
}

// Loads a new Sprite and Palette by Overwriting an exisitng objects sprite and palette
void LoadToExistingObject(Object * const a_pObject, const unsigned short *a_pusPallete, const u16  a_uiPalleteLength, const unsigned short *a_pusTiles, const u16  a_uiTilesLength)
{
	// Overwrite Sprite and Palette
	dma3_cpy(palette_sp_block_address(a_pObject->uiObjID), a_pusPallete, a_uiPalleteLength);
	dma3_cpy(sprite_tile_block_address(a_pObject->uiObjTileStartAddress), a_pusTiles, a_uiTilesLength);
}

// Assigns object variables based on Loader Variables
void SetupObject(Loader * const a_pLoader, Object * const a_pObject)
{
	// Initialise Object
	a_pObject->uiObjID = a_pLoader->uiObjCount;
	a_pObject->uiObjTileStartAddress = a_pLoader->uiObjallocatedTiles;
	a_pObject->poaAttrib = &MEM_OAM[a_pLoader->uiObjCount];
}