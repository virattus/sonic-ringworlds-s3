#ifndef __SSEE_STATE_H__
#define __SSEE_STATE_H__

#include <stdint.h>
#include <stdbool.h>


typedef void (*GameStateFunc)(void);

typedef struct 
{
	GameStateFunc InitState;
	GameStateFunc FreeState;
	GameStateFunc UpdateState;
	
} gamestate_t;


gamestate_t GameState_Current(void);

bool GameState_Push(gamestate_t newState);

void GameState_Pop(void);

void GameState_UpdateStack(void);


#endif
