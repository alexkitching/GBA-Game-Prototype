#ifndef __LOADER_H__
#define __LOADER_H__

/*
----------------------------------------------------------/
----- Project: GBA Assignment							  /
----- File:    loader.h									  /
----------------------------------------------------------/
----- Desc: Declares the Loader Struct and Functions.	  /
-----       The Loader handles the loading in of Objects. /
----------------------------------------------------------/
----- Author: Alex Kitching								  /
----------------------------------------------------------/
*/

#include "gba.h"
#include "object.h"

// MAX OBJECTS/TILES
#define MAXOBJ 128
#define MAXOBJ_TILES 1024

// Player Start Variables
#define PLAYER_STARTPOSX 30
#define PLAYER_STARTPOSY 68
#define PLAYER_PRIORITY 4

// OBJ IDs - Defined by order in which loaded
#define PLAYER_ID 0
#define FLAME_ID 1
#define SNOW_ID 2

typedef struct Loader
{
	// Object Array Initialised to 128 allowing for later expansion as well as dyanmic expansion of objects
	Object aoObjArray[MAXOBJ];

	// Object Memory
	u8 uiObjCount;
	// Current Tiles Allocated 4Bytes Each
	u16 uiObjallocatedTiles;

	// Padding
	u8 pad;
}Loader; // 4100 Bytes

extern void LoadData(Loader * const a_loader);
void LoadObject(Loader * const a_loader, const unsigned short *a_pusPallete, const u16  a_uiPalleteLength, const unsigned short *a_pusTiles, const u16  a_uiTilesLength);
void LoadToExistingObject(Object * const a_pObject, const unsigned short *a_pusPallete, const u16  a_uiPalleteLength, const unsigned short *a_pusTiles, const u16  a_uiTilesLength);
void SetupObject(Loader * const a_loader, Object * const a_pObject);

#endif // !__LOADER_H__
