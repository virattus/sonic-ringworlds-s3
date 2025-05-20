#include "titlestate.h"




void TitleState_Init(void)
{
}


void TitleState_Free(void)
{
}


void TitleState_Update(void)
{
}


gamestate_t Get_TitleState(void)
{
	return (gamestate_t)
	{
		.InitState = TitleState_Init,
		.FreeState = TitleState_Free,
		.UpdateState = TitleState_Update,
	};
}
