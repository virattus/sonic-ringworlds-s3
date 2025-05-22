#include "cameramovement.h"

#include <object/TPCamera/TPCamera.h>


void CameraMovement_Init(void)
{
}


void CameraMovement_Free(void)
{
}


void CameraMovement_Update(void)
{
}



gamestate_t Get_CameraMovementState(void)
{
	return (gamestate_t)
	{
		.InitState = CameraMovement_Init,
		.FreeState = CameraMovement_Free,
		.UpdateState = CameraMovement_Update,
	};
}

