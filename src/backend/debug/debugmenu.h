#ifndef __SSEE_DEBUG_MENU_H__
#define __SSEE_DEBUG_MENU_H__


#include <stdint.h>


#define DEBUG_CONTROLLER_PORT			(1)

//list of possible states for debug menu
//Enum wasn't good enough
#define DB_STATE_DISPLAY_OFF		(0)
#define DB_STATE_SYSTEM				(DB_STATE_DISPLAY_OFF + 1)
#define DB_STATE_CHEATS				(DB_STATE_SYSTEM + 1)
#define DB_STATE_MEMORY_USAGE		(DB_STATE_CHEATS + 1)
#define DB_STATE_VDP1				(DB_STATE_MEMORY_USAGE + 1)
#define DB_STATE_COMMAND_TABLES		(DB_STATE_VDP1 + 1)
#define DB_STATE_VDP2				(DB_STATE_COMMAND_TABLES + 1)
#define DB_STATE_SCSP				(DB_STATE_VDP2 + 1)
#define DB_STATE_CONSOLE			(DB_STATE_SCSP + 1)
#define DB_STATE_COUNT				(DB_STATE_CONSOLE + 1)


void DebugMenu_IncrementState();
void DebugMenu_DecrementState();

void DebugMenu_SetState(uint16_t state);

void DebugMenu_Update();


#endif //__SSEE_DEBUG_MENU_H__
