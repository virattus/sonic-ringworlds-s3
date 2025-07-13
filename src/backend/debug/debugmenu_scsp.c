#include "debugmenu.h"
#include "debug_window.h"


void DebugSCSP_Update(smpc_peripheral_digital_t* digital)
{	
	DebugWindow_ClearCanvas();
	
	int16_vec2_t width = {
		.x = 0,
		.y = 0,
	};
	
	int16_vec2_t height = {
		.x = DEBUGWINDOW_MAX_WIDTH,
		.y = DEBUGWINDOW_MAX_HEIGHT,
	};
	
	DebugWindow_DrawWindow(&width, &height, "SCSP");	
}
