#include "levelstate.h"



void LevelState_Init(void)
{
}


void LevelState_Free(void)
{
}


void LevelState_Update(void)
{

}


gamestate_t Get_LevelState(void)
{
	return (gamestate_t)
	{
		.InitState = LevelState_Init,
		.FreeState = LevelState_Free,
		.UpdateState = LevelState_Update,
	};
}
