#ifndef __RINGWORLDS_LEVEL_STATE_H__
#define __RINGWORLDS_LEVEL_STATE_H__


#include <globals.h>
#include <backend/state.h>

#include "object/ring/ring_bounce.h"
#include "collision/polygon.h"
#include "collision/collisionWorld.h"
#include "collision/octree.h"


void LevelState_Init(void);
void LevelState_Free(void);
void LevelState_Update(void);


gamestate_t Get_LevelState(void);



#endif
