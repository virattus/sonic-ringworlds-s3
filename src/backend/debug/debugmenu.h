#ifndef __SSEE_DEBUG_MENU_H__
#define __SSEE_DEBUG_MENU_H__


typedef enum
{
	DISPLAY_OFF,
	DISPLAY_MEMORY_USAGE,
	
	
} debugmenu_state_t;


void DebugMenu_SetState(debugmenu_state_t state);

void DebugMenu_Draw();


#endif //__SSEE_DEBUG_MENU_H__
