#include "modelloading.h"



void ModelLoadState_Init(void)
{
}


void ModelLoadState_Free(void)
{
}


void ModelLoadState_Update(void)
{
}


gamestate_t Get_ModelLoadState(void)
{
	return (gamestate_t)
	{
		.InitState = ModelLoadState_Init,
		.FreeState = ModelLoadState_Free,
		.UpdateState = ModelLoadState_Update,
	};
}
