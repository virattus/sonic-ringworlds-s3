#include "debugmenu_cmdt.h"

#include "debug_window.h"
#include "debugmenu.h"


#define DBCMDT_OPTION_START				(0)
#define DBCMDT_OPTION_FORCE_WIREFRAME	(DBCMDT_OPTION_START + 1)
#define DBCMDT_OPTION_CURR_CMDT			(DBCMDT_OPTION_FORCE_WIREFRAME + 1)
#define DBCMDT_OPTION_END				(DBCMDT_OPTION_CURR_CMDT + 1)

#define DEFAULT_MAX_CMDTS				(0)


int16_t db_cmdt_option = DBCMDT_OPTION_START + 1;
uint16_t db_cmdt_option_values[DBCMDT_OPTION_END] = {0};
uint16_t db_cmdt_option_values_max[DBCMDT_OPTION_END] = {
	0, //DBCMDT_OPTION_START
	1,
	DEFAULT_MAX_CMDTS,
};


vdp1_cmdt_list_t* currentList = NULL;


static void DebugCmdt_Option_ShiftUp(void)
{
	db_cmdt_option -= 1;
	if(db_cmdt_option == DBCMDT_OPTION_START)
	{
		db_cmdt_option = DBCMDT_OPTION_END - 1;
	}
}


static void DebugCmdt_Option_ShiftDown(void)
{
	db_cmdt_option += 1;
	if(db_cmdt_option == DBCMDT_OPTION_END)
	{
		db_cmdt_option = DBCMDT_OPTION_START + 1;
	}
}


static void DebugCmdt_Option_Increment(void)
{
	db_cmdt_option_values[db_cmdt_option] += 1;
	
	if(db_cmdt_option_values[db_cmdt_option] ==
		(db_cmdt_option_values_max[db_cmdt_option] + 1))
	{
		db_cmdt_option_values[db_cmdt_option] = 0;
	}
}


static void DebugCmdt_Option_Decrement(void)
{
	if(db_cmdt_option_values[db_cmdt_option] == 0)
	{
		db_cmdt_option_values[db_cmdt_option] = db_cmdt_option_values_max[db_cmdt_option];
	}
	else
	{
		db_cmdt_option_values[db_cmdt_option] -= 1;
	}
}


static void DebugCmdt_Draw_Type(const vdp1_cmdt_t* cmdt, int16_vec2_t* pos)
{
	char LineBuffer[DEBUGWINDOW_MAX_WIDTH];

	snprintf(LineBuffer, DEBUGWINDOW_MAX_WIDTH, 
			"debugcmdt_draw_type called ");
				
	DebugWindow_DrawLine(pos, LineBuffer);
}


void DebugCmdt_Register(vdp1_cmdt_list_t* list)
{
	currentList = list;
	db_cmdt_option_values_max[DBCMDT_OPTION_CURR_CMDT] = list->count;
}


void DebugCmdt_Update(smpc_peripheral_digital_t* digital)
{
	if(digital->held.button.up)
	{
		DebugCmdt_Option_ShiftUp();
	}
	else if(digital->held.button.down)
	{
		DebugCmdt_Option_ShiftDown();
	}
	else if(digital->held.button.left)
	{
		DebugCmdt_Option_Decrement();
	}
	else if(digital->held.button.right)
	{
		DebugCmdt_Option_Increment();
	}
	
	
	DebugWindow_ClearCanvas();
	
	int16_vec2_t pos = {
		.x = 0,
		.y = 0,
	};	
	
	char LineBuffer[DEBUGWINDOW_MAX_WIDTH];
	
	DebugWindow_DrawLine(&pos, "DEBUG | VDP1 COMMAND TABLES ");
	pos.y += 2;
	
	//If there's no cmdt list, draw debug message and quit
	if(currentList == NULL)
	{
		snprintf(LineBuffer, DEBUGWINDOW_MAX_WIDTH, 
			"     Error: Command table list null ");
				
		DebugWindow_DrawLine(&pos, LineBuffer);
		
		return;
	}
	
	snprintf(LineBuffer, DEBUGWINDOW_MAX_WIDTH, 
		"     Total CMDTs: %6u ", currentList->count);
	DebugWindow_DrawLine(&pos, LineBuffer);
	pos.y += 2;
	
	//Menu options
	
	snprintf(LineBuffer, DEBUGWINDOW_MAX_WIDTH,
		"   %c FORCE WIREFRAME: %s ", 
		db_cmdt_option == DBCMDT_OPTION_FORCE_WIREFRAME ? '@' : ' ',
		db_cmdt_option_values[DBCMDT_OPTION_FORCE_WIREFRAME] ? "ON" : "OFF");
	DebugWindow_DrawLine(&pos, LineBuffer);
	pos.y += 2;
	
	snprintf(LineBuffer, DEBUGWINDOW_MAX_WIDTH,
		"   %c CURRENT CMDT: %4u ", 
		db_cmdt_option == DBCMDT_OPTION_CURR_CMDT ? '@' : ' ',
		db_cmdt_option_values[DBCMDT_OPTION_CURR_CMDT]);
	DebugWindow_DrawLine(&pos, LineBuffer);
	pos.y += 2;
	
	//Current Cmdt display
	vdp1_cmdt_t* currentCmdt = &currentList->cmdts[db_cmdt_option_values[DBCMDT_OPTION_CURR_CMDT]];
	
	DebugCmdt_Draw_Type(currentCmdt, &pos);
	pos.y += 1;
}
