#include "debugmenu.h"
#include "debug_window.h"





void DebugVDP1_Update(void)
{
	smpc_peripheral_digital_t digital;
	smpc_peripheral_process();
	smpc_peripheral_digital_port(DEBUG_CONTROLLER_PORT, &digital);
	
	
	if(digital.released.button.l)
	{
		//DebugMenu_DecrementState();
	}
	if(digital.released.button.r)
	{
		//DebugMenu_IncrementState();
	}
	
	
	
	DebugWindow_ClearCanvas();
	
	int16_vec2_t width = {
		.x = 0,
		.y = 0,
	};
	
	int16_vec2_t height = {
		.x = DEBUGWINDOW_MAX_WIDTH,
		.y = DEBUGWINDOW_MAX_HEIGHT,
	};
	
	DebugWindow_DrawWindow(&width, &height, "VDP1");
}
