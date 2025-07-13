#include "debugmenu.h"

#include "debug_window.h"

#include "debugmenu_system.h"
#include "debugmenu_gameplay.h"
#include "debugmenu_memory.h"
#include "debugmenu_vdp1.h"
#include "debugmenu_cmdt.h"
#include "debugmenu_vdp2.h"
#include "debugmenu_scsp.h"
#include "debugmenu_console.h"

#include <yaul.h>

#include <assert.h>


static uint16_t currentDebugState;


void DebugMenu_IncrementState()
{
	currentDebugState += 1;
	if(currentDebugState == DB_STATE_COUNT)
	{
		currentDebugState = DB_STATE_DISPLAY_OFF + 1;
	}
}


void DebugMenu_DecrementState()
{
	currentDebugState -= 1;
	if(currentDebugState == DB_STATE_DISPLAY_OFF)
	{
		currentDebugState = DB_STATE_COUNT - 1;
	}
}


void DebugMenu_SetState(uint16_t state)
{
	assert(state < DB_STATE_COUNT);
	
	currentDebugState = state;
}


void DebugMenu_Update()
{
	smpc_peripheral_digital_t digital;
	smpc_peripheral_process();
	smpc_peripheral_digital_port(DEBUG_CONTROLLER_PORT, &digital);
	
	
	if(digital.released.button.l)
	{
		DebugMenu_DecrementState();
	}
	if(digital.released.button.r)
	{
		DebugMenu_IncrementState();
	}
	
	
	switch(currentDebugState)
	{
	case DB_STATE_DISPLAY_OFF:
		break;
	case DB_STATE_SYSTEM:
		DebugSystem_Update(&digital);
		break;
	case DB_STATE_CHEATS:
		DebugGameplay_Update(&digital);
		break;
	case DB_STATE_MEMORY_USAGE:
		DebugMemoryUsage_Update(&digital);
		break;
	case DB_STATE_VDP1:
		DebugVDP1_Update(&digital);
		break;
	case DB_STATE_COMMAND_TABLES:
		DebugCmdt_Update(&digital);
		break;
	case DB_STATE_VDP2:
		DebugVDP2_Update(&digital);
		break;
	case DB_STATE_SCSP:
		DebugSCSP_Update(&digital);
		break;
	case DB_STATE_CONSOLE:
		DebugConsole_Update(&digital);
		break;
	default:
		dbgio_printf("Debug Menu: Invalid state\n");
		break;
	}
	
	DebugWindow_DrawCanvas();
}
