#include "debugmenu_console.h"

#include "debug_window.h"

#define MAX_LINES		(24)

char LineBuffers[DEBUGWINDOW_MAX_WIDTH * MAX_LINES] = {0};
uint16_t CurrentLine = 0;



void DebugConsole_Update(smpc_peripheral_digital_t* digital)
{
	DebugWindow_ClearCanvas();
	
	int16_vec2_t pos = {
		.x = 0,
		.y = 0,
	};
	
	DebugWindow_DrawLine(&pos, "DEBUG | CONSOLE");
	pos.y += 2;
	
	uint16_t offset = CurrentLine;
	for(int i = 0; i < MAX_LINES; i++)
	{
		pos.y += 1;
		offset += 1;
		if(offset >= MAX_LINES)
		{
			offset = 0;
		}
		
		DebugWindow_DrawLine(&pos, &LineBuffers[DEBUGWINDOW_MAX_WIDTH * offset]);
	}
}


void DebugConsole_WriteLine(const char* string)
{
	snprintf(&LineBuffers[DEBUGWINDOW_MAX_WIDTH * CurrentLine], DEBUGWINDOW_MAX_WIDTH, string);
	
	CurrentLine += 1;
	if(CurrentLine >= MAX_LINES)
	{
		CurrentLine = 0;
	}
}

