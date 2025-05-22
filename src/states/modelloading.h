#ifndef __RINGWORLDS_MODEL_LOAD_TEST_STATE_H__
#define __RINGWORLDS_MODEL_LOAD_TEST_STATE_H__


#include <backend/state.h>
#include <globals.h>


void ModelLoadState_Init(void);
void ModelLoadState_Free(void);
void ModelLoadState_Update(void);


gamestate_t Get_ModelLoadState(void);


#endif
