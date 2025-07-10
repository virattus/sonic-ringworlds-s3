#include "debugmenu.h"

#include "debugmenu_system.h"
#include "debugmenu_gameplay.h"
#include "debugmenu_memory.h"
#include "debugmenu_vdp1.h"
#include "debugmenu_vdp2.h"
#include "debugmenu_scsp.h"

#include <yaul.h>

#include <assert.h>


static int currentDebugState;


void DebugMenu_IncrementState()
{
	currentDebugState += 1;
	if(currentDebugState == STATE_COUNT)
	{
		currentDebugState = DISPLAY_MEMORY_USAGE;
	}
}


void DebugMenu_DecrementState()
{
	currentDebugState -= 1;
	if(currentDebugState == DISPLAY_MEMORY_USAGE)
	{
		currentDebugState = STATE_COUNT - 1;
	}
}


void DebugMenu_SetState(debugmenu_state_t state)
{
	currentDebugState = state;
}


void DebugMenu_Update()
{
	switch(currentDebugState)
	{
	case DISPLAY_OFF:
		break;
	case DISPLAY_SYSTEM:
		DebugSystem_Update();
		break;
	case DISPLAY_GAMEPLAY_CHEATS:
		DebugGameplay_Update();
		break;
	case DISPLAY_MEMORY_USAGE:
		DebugMemoryUsage_Update();
		break;
	case DISPLAY_VDP1:
		DebugVDP1_Update();
		break;
	case DISPLAY_VDP2:
		DebugVDP2_Update();
		break;
	case DISPLAY_SCSP:
		DebugSCSP_Update();
		break;
	default:
		dbgio_printf("Debug Menu: Invalid state\n");
		break;
	}
}
