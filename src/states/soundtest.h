#ifndef __RINGWORLDS_SOUND_TEST_STATE_H__
#define __RINGWORLDS_SOUND_TEST_STATE_H__


#include <backend/state.h>
#include <globals.h>


void SoundTestState_Init(void);
void SoundTestState_Free(void);
void SoundTestState_Update(void);


gamestate_t Get_SoundTestState(void);


#endif
