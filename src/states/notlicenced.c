#include "notlicenced.h"


void NotLicencedState_Init(void)
{
}


void NotLicencedState_Free(void)
{
}


void NotLicencedState_Update(void)
{
}


gamestate_t Get_NotLicencedState(void)
{
	return (gamestate_t)
	{
		.InitState = NotLicencedState_Init,
		.FreeState = NotLicencedState_Free,
		.UpdateState = NotLicencedState_Update,
	};
}
