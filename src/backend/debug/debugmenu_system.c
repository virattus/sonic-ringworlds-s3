#include "debugmenu_system.h"

#include "debug_window.h"
#include "debugmenu.h"


typedef struct
{
	uint32_t ticks;
	uint32_t maxTicks;
	
} perfLevel_t;


perfLevel_t levels[PERF_COUNT];


void DebugSystem_SetPerfLevel(uint16_t perfID, uint32_t ticks, uint32_t maxTicks)
{
	levels[perfID].ticks = ticks;
	levels[perfID].maxTicks = maxTicks;
}


void DebugSystem_Update(smpc_peripheral_digital_t* digital)
{
	DebugWindow_ClearCanvas();
	
	int16_vec2_t pos = {
		.x = 0,
		.y = 0,
	};
	
	char lineBuffer[DEBUGWINDOW_MAX_WIDTH];
	
	DebugWindow_DrawLine(&pos, "DEBUG | SYSTEM OVERVIEW");
	
	pos.y = 5;
	snprintf(lineBuffer, DEBUGWINDOW_MAX_WIDTH, "    Master CPU: %10u(%10u)", 
		levels[PERF_CPU_MASTER].ticks, 
		levels[PERF_CPU_MASTER].maxTicks);
		
	DebugWindow_DrawLine(&pos, lineBuffer);
	
	pos.y += 1;
	DebugWindow_DrawLine(&pos, lineBuffer);
	
	pos.y += 4;
	snprintf(lineBuffer, DEBUGWINDOW_MAX_WIDTH, "     Slave CPU: %10u(%10u)", 
		levels[PERF_CPU_SLAVE].ticks, 
		levels[PERF_CPU_SLAVE].maxTicks);
		
	DebugWindow_DrawLine(&pos, lineBuffer);
	
	pos.y += 1;
	DebugWindow_DrawLine(&pos, lineBuffer);
	
}
