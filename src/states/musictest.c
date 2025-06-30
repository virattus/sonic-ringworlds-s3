#include "musictest.h"


void MusicTestState_Init(void)
{
}


void MusicTestState_Free(void)
{
}


void MusicTestState_Update(void)
{
}


gamestate_t Get_MusicTestState(void)
{
	return (gamestate_t)
	{
		.InitState = MusicTestState_Init,
		.FreeState = MusicTestState_Free,
		.UpdateState = MusicTestState_Update,
	};
}
