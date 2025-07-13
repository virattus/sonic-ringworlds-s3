#include "debugmenu_cmdt.h"

#include "debug_window.h"
#include "debugmenu.h"


#define DBCMDT_OPTION_START				(0)
#define DBCMDT_OPTION_PAUSE_VDP1		(DBCMDT_OPTION_START + 1)
#define DBCMDT_OPTION_FORCE_WIREFRAME	(DBCMDT_OPTION_PAUSE_VDP1 + 1)
#define DBCMDT_OPTION_CURR_CMDT			(DBCMDT_OPTION_FORCE_WIREFRAME + 1)
#define DBCMDT_OPTION_END				(DBCMDT_OPTION_CURR_CMDT + 1)

#define DEFAULT_MAX_CMDTS				(0)


int16_t db_cmdt_option = DBCMDT_OPTION_START + 1;
uint16_t db_cmdt_option_values[DBCMDT_OPTION_END] = {0};
uint16_t db_cmdt_option_values_max[DBCMDT_OPTION_END] = {
	0, //DBCMDT_OPTION_START
	1,
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


static void DebugCmdt_Draw_Type(const vdp1_cmdt_t* cmdt, int16_vec2_t* pos);


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
		"   %c PAUSE VDP1: %s ", 
		db_cmdt_option == DBCMDT_OPTION_PAUSE_VDP1 ? '@' : ' ',
		db_cmdt_option_values[DBCMDT_OPTION_PAUSE_VDP1] ? "ON" : "OFF");
	DebugWindow_DrawLine(&pos, LineBuffer);
	pos.y += 2;
	
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


// CMDT handling

#define CMDT_CTRL_END_TABLE			(0x8000)
#define CMDT_CTRL_TYPE_BITS			(0xF)
#define CMDT_CTRL_SYS_CLIP			(0x9)
#define CMDT_CTRL_USER_CLIP			(0x8)
#define CMDT_CTRL_LOCAL_COORD		(0xA)
#define CMDT_CTRL_NORMAL_SPRITE		(0x0)
#define CMDT_CTRL_SCALED_SPRITE		(0x1)
#define CMDT_CTRL_DISTORTED_SPRITE	(0x2)
#define CMDT_CTRL_POLYGON			(0x4)
#define CMDT_CTRL_POLYLINE			(0x5)
#define CMDT_CTRL_LINE				(0x6)

static void DebugCmdt_Draw_Type(const vdp1_cmdt_t* cmdt, int16_vec2_t* pos)
{
	
	uint16_t cmd_ctrl = cmdt->cmd_ctrl;

	char LineBuffer[DEBUGWINDOW_MAX_WIDTH];
	char TypeName[20];
	
	//early break if it's an end table
	if(cmd_ctrl & CMDT_CTRL_END_TABLE)
	{
		snprintf(TypeName, 20, "END TABLE");
		snprintf(LineBuffer, DEBUGWINDOW_MAX_WIDTH, 
			"  CMDT Type %s ", TypeName);
		DebugWindow_DrawLine(pos, LineBuffer);
		return;
	}
	
	uint16_t ctrl_bits = cmd_ctrl & CMDT_CTRL_TYPE_BITS;

	if(ctrl_bits == CMDT_CTRL_SYS_CLIP)
	{
		snprintf(TypeName, 20, "SYSTEM CLIPPING");
	}
	else if(ctrl_bits == CMDT_CTRL_USER_CLIP)
	{
		snprintf(TypeName, 20, "USER CLIPPING");		
	}
	else if(ctrl_bits == CMDT_CTRL_LOCAL_COORD)
	{
		snprintf(TypeName, 20, "LOCAL COORD");
	}
	else if(ctrl_bits == CMDT_CTRL_NORMAL_SPRITE)
	{
		snprintf(TypeName, 20, "NORMAL SPRITE");
	}
	else if(ctrl_bits == CMDT_CTRL_SCALED_SPRITE)
	{
		snprintf(TypeName, 20, "SCALED SPRITE");
	}
	else if(ctrl_bits == CMDT_CTRL_DISTORTED_SPRITE)
	{
		snprintf(TypeName, 20, "DISTORTED SPRITE");
	}
	else if(ctrl_bits == CMDT_CTRL_POLYGON)
	{
		snprintf(TypeName, 20, "POLYGON");
	}
	else if(ctrl_bits == CMDT_CTRL_POLYLINE)
	{
		snprintf(TypeName, 20, "POLYLINE");
	}
	else if(ctrl_bits == CMDT_CTRL_LINE)
	{
		snprintf(TypeName, 20, "LINE");
	}
	
	

	snprintf(LineBuffer, DEBUGWINDOW_MAX_WIDTH, 
			"  CMDT Type %s ", TypeName);
	
	DebugWindow_DrawLine(pos, LineBuffer);
}
