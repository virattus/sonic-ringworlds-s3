#ifndef __SSEE_COLLISION_WORLD_H__
#define __SSEE_COLLISION_WORLD_H__


#include "collision.h"
#include "col_resp.h"

#include "aabb.h"
#include "line.h"
#include "octree.h"
#include "plane.h"
#include "polygon.h"
#include "sphere.h"

#include <yaul.h>
#include <stdint.h>
#include <stdbool.h>


typedef enum
{
	PLAYER,
	ENEMY,
	CAMERA,
	
} colworld_obj_type_t;


bool colworld_BindOctree(octree_t* octreeRoot);
void colworld_RemoveOctree(octree_t* octreeRoot);

//TODO how do we pass this in? A void*? Separate functions for each type?
uint8_t colworld_BindObject();

uint8_t colworld_GetObjCol_Count(uint8_t objID);
col_resp_t* colworld_GetObjectCol_Resp(uint8_t objID, uint8_t colID);


bool colworld_CheckCollisionSphere(
	const col_sphere_t* s0, 
	const polygon_mask_types_t sphereMask,
	col_resp_t* resp_array, 
	uint8_t maxCollisions);

bool colworld_CheckCollisionLine(
	const col_line_t* l0, 
	const polygon_mask_types_t lineMask,
	col_resp_t* resp_array, 
	uint8_t maxCollisions);



#endif
