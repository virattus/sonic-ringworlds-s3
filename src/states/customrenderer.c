#include "customrenderer.h"




void CustomRendererState_Init(void)
{
}


void CustomRendererState_Free(void)
{
}


void CustomRendererState_Update(void)
{
}


gamestate_t Get_CustomRendererState(void)
{
	return (gamestate_t)
	{
		.InitState = CustomRendererState_Init,
		.FreeState = CustomRendererState_Free,
		.UpdateState = CustomRendererState_Update,
	};
}
