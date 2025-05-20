#ifndef __RINGWORLDS_TEST_COLLISION_STATE_H__
#define __RINGWORLDS_TEST_COLLISION_STATE_H__


#include <backend/state.h>
#include <globals.h>


void TestCollisionState_Init(void);
void TestCollisionState_Free(void);
void TestCollisionState_Update(void);


gamestate_t Get_TestCollisionState(void);



#endif
