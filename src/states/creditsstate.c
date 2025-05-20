#include "creditsstate.h"



void CreditsState_Init(void)
{
}


void CreditsState_Free(void)
{
}


void CreditsState_Update(void)
{

}


gamestate_t Get_CreditsState(void)
{
	return (gamestate_t)
	{
		.InitState = CreditsState_Init,
		.FreeState = CreditsState_Free,
		.UpdateState = CreditsState_Update,
	};
}
