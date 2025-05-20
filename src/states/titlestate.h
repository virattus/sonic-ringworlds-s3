#ifndef __RINGWORLDS_TITLE_STATE_H__
#define __RINGWORLDS_TITLE_STATE_H__


#include <globals.h>

#include <backend/state.h>

#include "object/ring/ring_bounce.h"
#include "collision/polygon.h"
#include "collision/collisionWorld.h"
#include "collision/octree.h"


void TitleState_Init(void);
void TitleState_Free(void);
void TitleState_Update(void);


gamestate_t Get_TitleState(void);





#endif
