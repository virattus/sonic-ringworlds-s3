#include "debugmenu_cmdt.h"

#include "debug_window.h"
#include "debugmenu.h"


#define DBCMDT_OPTION_START				(0)
#define DBCMDT_OPTION_CURR_CMDT			(DBCMDT_OPTION_START + 1)
#define DBCMDT_OPTION_FORCE_WIREFRAME	(DBCMDT_OPTION_CURR_CMDT + 1)
#define DBCMDT_OPTION_END				(DBCMDT_OPTION_FORCE_WIREFRAME + 1)

#define DEFAULT_MAX_CMDTS				(0)


int16_t db_cmdt_option = DBCMDT_OPTION_START + 1;
uint16_t db_cmdt_option_values[DBCMDT_OPTION_END - 1] = {0};
uint16_t db_cmdt_option_values_max[DBCMDT_OPTION_END - 1] = {
	0,
	DEFAULT_MAX_CMDTS,
	1,
};

const int16_vec2_t LinePositions[] = {
	INT16_VEC2_INITIALIZER(0, 0),
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
	if((db_cmdt_option_values[db_cmdt_option] + 1) ==
		db_cmdt_option_values_max[db_cmdt_option])
	{
		db_cmdt_option_values[db_cmdt_option] = 0;
	}
	else
	{
		db_cmdt_option_values[db_cmdt_option] += 1;
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
		db_cmdt_option_values[db_cmdt_option] += 1;
	}	
}


static void DebugCmdt_Draw_Type(const vdp1_cmdt_t* cmdt)
{
	
}


void DebugCmdt_Register(vdp1_cmdt_list_t* list)
{
	currentList = list;
	db_cmdt_option_values_max[DBCMDT_OPTION_CURR_CMDT] = list->count;
}


void DebugCmdt_Update(smpc_peripheral_digital_t* digital)
{
	//Get rendering going first
	/*
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
	*/
	
	DebugWindow_ClearCanvas();
	
	int16_vec2_t width = {
		.x = 0,
		.y = 0,
	};
	
	int16_vec2_t height = {
		.x = DEBUGWINDOW_MAX_WIDTH,
		.y = DEBUGWINDOW_MAX_HEIGHT,
	};
	
	DebugWindow_DrawWindow(&width, &height, "COMMAND TABLES");	
	
	
	
	if(currentList == NULL)
	{
		char output[20];
		
		snprintf(output, 20, "test");
		
		//snprintf(output, 20, "Error: command tables not bound");
		
		DebugWindow_DrawLine(&LinePositions[0], output);
		
		return;
	}
}
