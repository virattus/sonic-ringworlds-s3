#include "consoletest.h"

#include "backend/debug/debug.h"


uint16_t accum = 0;
uint16_t lineNum = 0;

static void ConsoleTest_Init(void)
{
}


static void ConsoleTest_Free(void)
{
}


static void ConsoleTest_Update(void)
{		
	accum += 1;
	if(accum > 200)
	{
		accum = 0;
		
		char line[30];
		snprintf(line, 30, "test line %6u", lineNum);
		DebugConsole_WriteLine(line);
		
		lineNum += 1;
	}
}



gamestate_t Get_ConsoleTestState(void)
{
	return (gamestate_t)
	{
		.InitState = ConsoleTest_Init,
		.FreeState = ConsoleTest_Free,
		.UpdateState = ConsoleTest_Update,
	};
}
