#include "debugmenu_cmdt.h"

#include "debug_window.h"
#include "debugmenu.h"
#include "globals.h"

#define DBCMDT_FREEZE_CMDTS			(0)
#define DBCMDT_FORCE_WIREFRAME		(1)
#define DBCMDT_DISABLE_LIGHTING		(2)
#define DBCMDT_DISABLE_FOG			(3)
#define DBCMDT_DISABLE_LANTERN		(4)
#define DBCMDT_CURR_CMDT			(5)
#define DBCMDT_OPTIONS_COUNT		(6)

static const uint8_t dbcmdt_options[DBCMDT_OPTIONS_COUNT] = {
	GAME_FLAG_FREEZE_CMDTS,
	GAME_FLAG_FORCE_WIREFRAME,
	GAME_FLAG_DISABLE_LIGHTING,
	GAME_FLAG_DISABLE_FOG,
	GAME_FLAG_DISABLE_LANTERN,
	GAME_FLAG_CURR_CMDTS,
};

uint8_t dbcmdt_option_selected = 0;


vdp1_cmdt_list_t* currentList = NULL;


static void DebugCmdt_Option_ShiftUp(void)
{
	if(dbcmdt_option_selected == 0)
	{
		dbcmdt_option_selected = DBCMDT_OPTIONS_COUNT - 1;
	}
	else
	{
		dbcmdt_option_selected -= 1;
	}
}


static void DebugCmdt_Option_ShiftDown(void)
{
	dbcmdt_option_selected += 1;
	if(dbcmdt_option_selected >= DBCMDT_OPTIONS_COUNT)
	{
		dbcmdt_option_selected = 0;
	}
}


static void DebugCmdt_Option_Increment(void)
{
	if(GameFlags[dbcmdt_options[dbcmdt_option_selected]] ==
		GameFlagsMax[dbcmdt_options[dbcmdt_option_selected]])
	{
		GameFlags[dbcmdt_options[dbcmdt_option_selected]] = 0;
	}
	else
	{
		GameFlags[dbcmdt_options[dbcmdt_option_selected]] += 1;
	}
}


static void DebugCmdt_Option_Decrement(void)
{
	if(GameFlags[dbcmdt_options[dbcmdt_option_selected]] == 0)
	{
		GameFlags[dbcmdt_options[dbcmdt_option_selected]] =
			GameFlagsMax[dbcmdt_options[dbcmdt_option_selected]];
	}
	else
	{
		GameFlags[dbcmdt_options[dbcmdt_option_selected]] -= 1;
	}
}


static void DebugCmdt_PrintCmdtInfo(const vdp1_cmdt_t* cmdt, int16_vec2_t* pos);


void DebugCmdt_Register(vdp1_cmdt_list_t* list)
{
	currentList = list;
	GameFlagsMax[GAME_FLAG_CURR_CMDTS] = list->count;
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
		snprintf(LineBuffer, sizeof(LineBuffer), 
			"     Error: Command table list null ");
				
		DebugWindow_DrawLine(&pos, LineBuffer);
		
		return;
	}
	
	snprintf(LineBuffer, sizeof(LineBuffer), 
		"     Total CMDTs: %6u ", currentList->count);
	DebugWindow_DrawLine(&pos, LineBuffer);
	pos.y += 2;
	
	//Menu options

	snprintf(LineBuffer, sizeof(LineBuffer),
		"   %c PAUSE VDP1: %s ", 
		dbcmdt_option_selected == DBCMDT_FREEZE_CMDTS ? '\x10' : ' ',
		GameFlags[GAME_FLAG_FREEZE_CMDTS] ? "ON" : "OFF");
	DebugWindow_DrawLine(&pos, LineBuffer);
	pos.y += 1;
	
	snprintf(LineBuffer, sizeof(LineBuffer),
		"   %c FORCE WIREFRAME: %s ", 
		dbcmdt_option_selected == DBCMDT_FORCE_WIREFRAME ? '\x10' : ' ',
		GameFlags[GAME_FLAG_FORCE_WIREFRAME] ? "ON" : "OFF");
	DebugWindow_DrawLine(&pos, LineBuffer);
	pos.y += 1;
	
	snprintf(LineBuffer, sizeof(LineBuffer),
		"   %c DISABLE LIGHTING: %s ", 
		dbcmdt_option_selected == DBCMDT_DISABLE_LIGHTING ? '\x10' : ' ',
		GameFlags[GAME_FLAG_DISABLE_LIGHTING] ? "ON" : "OFF");
	DebugWindow_DrawLine(&pos, LineBuffer);
	pos.y += 1;
	
	snprintf(LineBuffer, sizeof(LineBuffer),
		"   %c DISABLE FOG: %s ", 
		dbcmdt_option_selected == DBCMDT_DISABLE_FOG ? '\x10' : ' ',
		GameFlags[GAME_FLAG_DISABLE_FOG] ? "ON" : "OFF");
	DebugWindow_DrawLine(&pos, LineBuffer);
	pos.y += 1;
	
	snprintf(LineBuffer, sizeof(LineBuffer),
		"   %c DISABLE_LANTERN: %s ", 
		dbcmdt_option_selected == DBCMDT_DISABLE_LANTERN ? '\x10' : ' ',
		GameFlags[GAME_FLAG_DISABLE_LANTERN] ? "ON" : "OFF");
	DebugWindow_DrawLine(&pos, LineBuffer);
	pos.y += 2;
	
	snprintf(LineBuffer, sizeof(LineBuffer),
		"   %c CURRENT CMDT: %4u ", 
		dbcmdt_option_selected == DBCMDT_CURR_CMDT ? '\x10' : ' ',
		GameFlags[GAME_FLAG_CURR_CMDTS]);
	DebugWindow_DrawLine(&pos, LineBuffer);
	pos.y += 2;
	
	//Current Cmdt display
	vdp1_cmdt_t* currentCmdt = &currentList->cmdts[GameFlags[GAME_FLAG_CURR_CMDTS]];
	
	DebugCmdt_PrintCmdtInfo(currentCmdt, &pos);
	pos.y += 1;
}


// CMDT handling

#define CMDT_JUMP_MASK		(0x7000)
#define CMDT_JUMP_NEXT		(0x0)
#define CMDT_JUMP_ASSIGN	(0x1)
#define CMDT_JUMP_CALL		(0x2)
#define CMDT_JUMP_RETURN	(0x3)
#define CMDT_SKIP_NEXT		(0x4)
#define CMDT_SKIP_ASSIGN	(0x5)
#define CMDT_SKIP_CALL		(0x6)
#define CMDT_SKIP_RETURN	(0x7)

static void DebugCmdt_PrintCmdtJumpInfo(const vdp1_cmdt_t* cmdt, int16_vec2_t* pos)
{
	char LineBuffer[DEBUGWINDOW_MAX_WIDTH];
	char JumpBuffer[12];
	
	uint16_t jumpType = cmdt->cmd_ctrl & CMDT_JUMP_MASK;
	uint16_t jumpPos = cmdt->cmd_link;
	
	switch(jumpType)
	{
	case CMDT_JUMP_NEXT:
		strncpy(JumpBuffer, "JP_NEXT", sizeof(JumpBuffer));
		break;
	case CMDT_JUMP_ASSIGN:
		strncpy(JumpBuffer, "JP_ASSIGN", sizeof(JumpBuffer));
		break;
	case CMDT_JUMP_CALL:
		strncpy(JumpBuffer, "JP_CALL", sizeof(JumpBuffer));
		break;
	case CMDT_JUMP_RETURN:
		strncpy(JumpBuffer, "JP_RETURN", sizeof(JumpBuffer));
		break;
	case CMDT_SKIP_NEXT:
		strncpy(JumpBuffer, "SK_NEXT", sizeof(JumpBuffer));
		break;
	case CMDT_SKIP_ASSIGN:
		strncpy(JumpBuffer, "SK_ASSIGN", sizeof(JumpBuffer));
		break;
	case CMDT_SKIP_CALL:
		strncpy(JumpBuffer, "SK_CALL", sizeof(JumpBuffer));
		break;
	case CMDT_SKIP_RETURN:
		strncpy(JumpBuffer, "SK_RETURN", sizeof(JumpBuffer));
		break;
	default:
		strncpy(JumpBuffer, "INVALID", sizeof(JumpBuffer));
		break;
	}
	
	snprintf(LineBuffer, sizeof(LineBuffer), "    JUMP: %s -> %u ", JumpBuffer, jumpPos);
	
	DebugWindow_DrawLine(pos, LineBuffer);
	pos->y += 1;
}


static void DebugCmdt_PrintCmdtQuadVertex(const vdp1_cmdt_t* cmdt, int16_vec2_t* pos)
{
	char LineBuffer[DEBUGWINDOW_MAX_WIDTH];
	
	snprintf(LineBuffer, sizeof(LineBuffer), "    POS: A(X): %6i | B(X): %6i ", cmdt->cmd_xa, cmdt->cmd_xb);
	
	DebugWindow_DrawLine(pos, LineBuffer);
	pos->y += 1;
	
	snprintf(LineBuffer, sizeof(LineBuffer), "         A(Y): %6i | B(Y): %6i ", cmdt->cmd_ya, cmdt->cmd_yb);
	
	DebugWindow_DrawLine(pos, LineBuffer);
	pos->y += 2;
	
	snprintf(LineBuffer, sizeof(LineBuffer), "         D(X): %6i | C(X): %6i ", cmdt->cmd_xd, cmdt->cmd_xc);
	
	DebugWindow_DrawLine(pos, LineBuffer);
	pos->y += 1;
	
	snprintf(LineBuffer, sizeof(LineBuffer), "         D(Y): %6i | C(Y): %6i ", cmdt->cmd_yd, cmdt->cmd_yc);
	
	DebugWindow_DrawLine(pos, LineBuffer);
	pos->y += 1;
	
}


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


static void DebugCmdt_PrintCmdtInfo(const vdp1_cmdt_t* cmdt, int16_vec2_t* pos)
{
	//It's easier to grab the position for the type of the cmdt
	//at the top rather than write everything in a linear fashion
	int16_vec2_t cmdtNamePos;
	int16_vec2_dup(pos, &cmdtNamePos);
	
	pos->y += 2;
	
	uint16_t cmd_ctrl = cmdt->cmd_ctrl;

	char LineBuffer[DEBUGWINDOW_MAX_WIDTH];
	char TypeName[20];
	
	//early break if it's an end table
	if(cmd_ctrl & CMDT_CTRL_END_TABLE)
	{
		snprintf(TypeName, sizeof(TypeName), "END TABLE");
		snprintf(LineBuffer, sizeof(LineBuffer), 
			"  CMDT Type %s ", TypeName);
		DebugWindow_DrawLine(&cmdtNamePos, LineBuffer);
		return;
	}

	DebugCmdt_PrintCmdtJumpInfo(cmdt, pos);
	
	//Now we handle the type of cmdt
	uint16_t ctrl_bits = cmd_ctrl & CMDT_CTRL_TYPE_BITS;

	if(ctrl_bits == CMDT_CTRL_SYS_CLIP)
	{
		snprintf(TypeName, sizeof(TypeName), "SYSTEM CLIPPING");
	}
	else if(ctrl_bits == CMDT_CTRL_USER_CLIP)
	{
		snprintf(TypeName, sizeof(TypeName), "USER CLIPPING");		
	}
	else if(ctrl_bits == CMDT_CTRL_LOCAL_COORD)
	{
		snprintf(TypeName, sizeof(TypeName), "LOCAL COORD");
	}
	else if(ctrl_bits == CMDT_CTRL_NORMAL_SPRITE)
	{
		snprintf(TypeName, sizeof(TypeName), "NORMAL SPRITE");
	}
	else if(ctrl_bits == CMDT_CTRL_SCALED_SPRITE)
	{
		snprintf(TypeName, sizeof(TypeName), "SCALED SPRITE");
	}
	else if(ctrl_bits == CMDT_CTRL_DISTORTED_SPRITE)
	{
		snprintf(TypeName, sizeof(TypeName), "DISTORTED SPRITE");
		DebugCmdt_PrintCmdtQuadVertex(cmdt, pos);
	}
	else if(ctrl_bits == CMDT_CTRL_POLYGON)
	{
		snprintf(TypeName, sizeof(TypeName), "POLYGON");
		DebugCmdt_PrintCmdtQuadVertex(cmdt, pos);
	}
	else if(ctrl_bits == CMDT_CTRL_POLYLINE)
	{
		snprintf(TypeName, sizeof(TypeName), "POLYLINE");
		DebugCmdt_PrintCmdtQuadVertex(cmdt, pos);
	}
	else if(ctrl_bits == CMDT_CTRL_LINE)
	{
		snprintf(TypeName, sizeof(TypeName), "LINE");
	}
	
	

	snprintf(LineBuffer, sizeof(LineBuffer), 
			"  CMDT Type %s ", TypeName);
	
	DebugWindow_DrawLine(&cmdtNamePos, LineBuffer);
}
