#include "state.h"


#define GAMESTATE_STACK_MAX_SIZE		(8)


gamestate_t StateStack[GAMESTATE_STACK_MAX_SIZE];
int8_t StateStackTop = 0;



gamestate_t GameState_Current(void)
{
	return StateStack[StateStackTop - 1];
}


bool GameState_Push(gamestate_t newState)
{
	if(StateStackTop >= GAMESTATE_STACK_MAX_SIZE - 1)
	{
		return false;
	}
	
	StateStack[StateStackTop] = newState;
	StateStackTop += 1;
	
	return true;
}


void GameState_Pop(void)
{
	if(StateStackTop <= 0)
	{
		return;
	}
	
	StateStackTop -= 1;
}


void GameState_UpdateStack(void)
{
	
	
}

