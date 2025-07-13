#include <yaul.h>

#include "debugmenu.h"
#include "debug_window.h"


unsigned int accumulator = 0;

void DebugMemoryUsage_Update(smpc_peripheral_digital_t* digital)
{
	if(digital->held.button.left)
	{
		accumulator = (accumulator - 1) & 0xFF;
	}
	
	if(digital->held.button.right)
	{
		accumulator = (accumulator + 1) & 0xFF;
	}

	if(digital->held.button.up)
	{
		accumulator = (accumulator - 10) & 0xFF;
	}
	
	if(digital->held.button.down)
	{
		accumulator = (accumulator + 10) & 0xFF;
	}
	
	//dbgio_printf("DebugMemoryUsage_Draw called\n");
	
	DebugWindow_ClearCanvas();
	
	int16_vec2_t width = {
		.x = 15,
		.y = 1,
	};
	
	int16_vec2_t height = {
		.x = DEBUGWINDOW_MAX_WIDTH - 5,
		.y = DEBUGWINDOW_MAX_HEIGHT,
	};
	
	DebugWindow_DrawWindow(&width, &height, "MEMORY VIEWER");
	
	/*
	DebugWindow_DrawWindow(
		INT16_VEC2_INITIALIZER(0, 0),
		INT16_VEC2_INITIALIZER(DEBUGWINDOW_MAX_WIDTH, DEBUGWINDOW_MAX_HEIGHT),
		"MEMORY VIEWER");
	*/
	
	DebugWindow_DrawCanvas();
	/*
	char charAcc[2];
	charAcc[0] = (char)accumulator;
	charAcc[1] = 0;
	dbgio_printf("ID: %d Char: ", accumulator);
	dbgio_printf(charAcc);
	*/
	
	//dbgio_puts("[7;1H\x04 16X16 FONT TEST! \x04\n");
	//dbgio_puts("\xBB testing \xBC\n");
}
