#ifndef __RINGWORLDS_CAMERA_MOVEMENT_STATE_H__
#define __RINGWORLDS_CAMERA_MOVEMENT_STATE_H__


#include <backend/state.h>


void CameraMovement_Init(void);
void CameraMovement_Free(void);
void CameraMovement_Update(void);


gamestate_t Get_CameraMovementState(void);



#endif
