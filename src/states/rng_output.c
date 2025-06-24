#include "rng_output.h"


void RNGOutputState_Init(void)
{
}


void RNGOutputState_Free(void)
{
}


void RNGOutputState_Update(void)
{
	dbgio_printf("RNG Output: %i\n", rand());
}


gamestate_t Get_RNGOutputState(void)
{
	return (gamestate_t)
	{
		.InitState = RNGOutputState_Init,
		.FreeState = RNGOutputState_Free,
		.UpdateState = RNGOutputState_Update,
	};
}
