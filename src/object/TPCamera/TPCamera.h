#ifndef __SSEE_THIRD_PERSON_CAMERA_H__
#define __SSEE_THIRD_PERSON_CAMERA_H__


#include <object/object.h>

#include <yaul.h>
#include <mic3d.h>



void ThirdPersCamera_Init(game_object_entry_t* entry);
void ThirdPersCamera_Free(game_object_entry_t* entry);
void ThirdPersCamera_Update(game_object_entry_t* entry);


#endif
