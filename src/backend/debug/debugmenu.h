#ifndef __SSEE_DEBUG_MENU_H__
#define __SSEE_DEBUG_MENU_H__


#define DEBUG_CONTROLLER_PORT		(1)

typedef enum
{
	DISPLAY_OFF = 0,
	DISPLAY_SYSTEM,
	DISPLAY_GAMEPLAY_CHEATS,
	DISPLAY_MEMORY_USAGE,
	DISPLAY_VDP1,
	DISPLAY_VDP2,
	DISPLAY_SCSP,
	STATE_COUNT
	
} debugmenu_state_t;


void DebugMenu_IncrementState();
void DebugMenu_DecrementState();

void DebugMenu_SetState(debugmenu_state_t state);

void DebugMenu_Update();


#endif //__SSEE_DEBUG_MENU_H__
