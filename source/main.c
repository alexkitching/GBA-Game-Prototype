#include "Intellisense.h"
#include "gba.h"
#include "game_manager.h"

int main()
{
	REG_DISPLAYCONTROL = VIDEOMODE_0 | ENABLE_OBJECTS | BGMODE_0 | BGMODE_1 | MAPPINGMODE_1D;
	GameManager manager;

	// Initialise the Game
	InitGame(&manager);

	bool ShouldQuit = false;
	while(!ShouldQuit)
	{
		// VSync
		vsync();
		// Poll Key Registers for Input
		PollKeys();

		// Update Game
		UpdateGame(&manager);

		// Quit Game
		if (keyDown(SELECT))
			ShouldQuit = true;
	}

	return 0;
}