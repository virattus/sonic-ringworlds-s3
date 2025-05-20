#ifndef __RINGWORLDS_CREDITS_STATE_H__
#define __RINGWORLDS_CREDITS_STATE_H__


#include <globals.h>
#include <backend/state.h>


void CreditsState_Init(void);
void CreditsState_Free(void);
void CreditsState_Update(void);


gamestate_t Get_CreditsState(void);


#endif
