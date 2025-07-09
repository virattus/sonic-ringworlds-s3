#include "debugmenu.h"


#include <assert.h>


static debugmenu_state_t currentDebugState;


void DebugMemoryUsage_Draw();


void DebugMenu_SetState(debugmenu_state_t state)
{
	currentDebugState = state;
}


void DebugMenu_Draw()
{
	switch(currentDebugState)
	{
	case DISPLAY_OFF:
		break;
	case DISPLAY_MEMORY_USAGE:
		DebugMemoryUsage_Draw();
		break;
	}
}
